#pragma once

#include <msgpack.hpp>
#include "Socket/SocketObject.h"
#include <string>
namespace kickflip
{
	#define		NET_CONNECT_PORT		(30002)

	static const char* header = "origin";
	// �f�[�^���M�^�C�v
	enum NET_SEND_FLAG{
		NET_SEND_NONRELIABLE		=	0x00000000,	// �����ŐM�����Ȃ��B�p�P�b�g�̓��X�g����\��������
		NET_SEND_RELIABLE			=	0x00000001,	// �M��������B�K���͂����ƑO��B
		NET_SEND_SYNC_BUCKET		=	0x00000004,	// bucketSync�Ɏg�p����C�x���g�B������g���ƃC�x���g����������Ԃŏ������邱�Ƃ��ł���B
		NET_SEND_NOW				=	0x00000008,	// �o�b�t�@�ɂ��߂��ɂ�������
		NET_SEND_LOW_PRIORITY		=	0x00000020,	// treat the message as low priority when queuing
		NET_SEND_NORMAL_PRIORITY	=	0x00000000,	// treat the message as normal priority when queuing (default)
		NET_SEND_HIGH_PRIORITY		=	0x00000040,	// treat the message as high priority when queuing
	};

	SmartPtr(NetObject);
	class NetObject : public ReferenceObject
	{
	public:
		NetObject()
			: ipAddr_(0)
			, port_(0)
			, tcp_(NULL)
			, udp_(NULL)
			, isLocal_(false)
			, isHost_(false)
		{}
		virtual ~NetObject() {}

	private:

		unsigned long		ipAddr_;		// IP�A�h���X
		unsigned short		port_;			// �ʐM�|�[�g
		TCPObjectRPtr		tcp_;			// TCP�ʐM�p�\�P�b�g
		UDPObjectRPtr		udp_;			// UDP�ʐM�p�\�P�b�g
		bool				isLocal_;		// ���[�J��(����)��
		bool				isHost_;		// �z�X�g(hosting)�� ����́A���������̏��ŊO���͂����m��K�v�͖���

		std::string			name_;

	private:
		SmartPtr(ChankData);
		class ChankData : public ReferenceObject
		{
		public:
			ChankData(const char* _command, const msgpack::sbuffer& _sbuf)
			{
				command_ = _command;
				sbuf_.write(_sbuf.data(),_sbuf.size());
			}
			virtual ~ChankData(){}
		private:
			ChankData(){}
		private:
			std::string command_;
			msgpack::sbuffer sbuf_;
		public:
			const char* GetSendBuffer() { return sbuf_.data();}
			unsigned int GetSendBufferSize() { return sbuf_.size();}
		};
		typedef std::vector<ChankDataRPtr> ChankList;

		ChankList sendChankReliableList_;		// ���M�o�b�t�@�M��������
		ChankList sendChankNonreliableList_;	// ���M�o�b�t�@�M�����Ȃ�
		ChankList recvChankReliableList_;		// ��M�o�b�t�@�M��������
		ChankList recvChankNonreliableList_;	// ��M�o�b�t�@�M�����Ȃ�

	public:
		

	private:
		friend class Network;
		bool pushDatatoChankbuffer( const char* _command, const msgpack::sbuffer& _sbuf, NET_SEND_FLAG _flag )
		{
			// �ʏ�̑��M�̓��[�J���ɂ͂��Ȃ�
			if(true == IsLocal()) return true;

			ChankDataRPtr chankdata = new ChankData(_command, _sbuf);

			if(_flag & NET_SEND_RELIABLE)
			{
				// �M�����ʐM
				sendChankReliableList_.push_back(chankdata);
			}else{
				// �����ʐM
				sendChankNonreliableList_.push_back(chankdata);
			}
			return true;

		}

		bool sendDatafromChankbuffer()
		{
			for( ChankList::iterator ite = sendChankReliableList_.begin(); sendChankReliableList_.end() != ite; ite++ )
			{
				const char* data = (*ite)->GetSendBuffer();
				unsigned int size = (*ite)->GetSendBufferSize();
				tcp_->SendData(data, size);
			}





			sendChankReliableList_.clear();
			return true;
		}
		bool recvDatafromChankbuffer()
		{
			msgpack::sbuffer rbuf;
			if(false == tcp_->RecvData(rbuf, 0))
			{
				std::string res(rbuf.data(),rbuf.size());
				NET_TRACE("recvDataChankbuffer error:%s",res.c_str());
				return false;

			}
			std::string res(rbuf.data(),rbuf.size());
			NET_TRACE("recvDataChankbuffer:%s",res.c_str());
			ChankDataRPtr chank = new ChankData("dummy",rbuf);
			recvChankReliableList_.push_back(chank);
			return true;
		}


	public:
		bool IsLocal() {return isLocal_;}
		const std::string& GetName(){ return name_;}

		bool Open( unsigned long _ipAddr, unsigned short _port, SOCKET _socket = NULL )
		{
			tcp_ = new TCPObject();
			if(false == tcp_->Open(_ipAddr,_port, _socket))
			{
				tcp_ = NULL;
				return false;
			}
			return true;

		}
		bool Open( const char* _ipAddr, unsigned short _port )
		{
			tcp_ = new TCPObject();
			if(false == tcp_->Open(_ipAddr,_port))
			{
				tcp_ = NULL;
				return false;
			}
			return true;

		}
		bool Connect(NetObjectRPtr connectNetObject)
		{
			if(NULL == tcp_) return false;
			if(NULL != connectNetObject)
				if(false == tcp_->Connect(false)) return false;
			
			ipAddr_ = tcp_->Sockaddr().sin_addr.s_addr;
			port_ = ntohs(tcp_->Sockaddr().sin_port);
			name_ = (inet_ntoa( *(struct in_addr*)&ipAddr_ ));

			if(NULL != connectNetObject)
			{
				// �ʐM�����UDP�\�P�b�g�͎����̃��m�����ʂŎg��
				isLocal_ = false;
				udp_ = connectNetObject->udp_;
			}else{
				// ���[�J���ɂ����쐬
				isLocal_ = true;
				udp_ = new UDPObject();
				if( false == udp_->Open( port_ ) )
				{
					udp_ = NULL;
					NET_TRACE( "UDPSocket Initialization failed!\n" );
					return false;
				}
			}

			return true;

		}
	};


	SmartPtr(Network);
	class Network :public ReferenceObject
	{
	private:
		struct Finalizer
		{
			virtual ~Finalizer(){ WSACleanup(); };
		};
		Finalizer finalizer_;
	public:
		static NetworkRPtr Get()
		{
			static NetworkRPtr instance_ = NULL;
			if(NULL == instance_) instance_ = new Network();
			return instance_;
		}
	public:
		SmartPtr(HttpObject);
		class HttpObject : public TCPObject
		{
		private :
			friend class Network;
			enum MODE{
				MODE_UNKNOWN,
				MODE_INIT,
				MODE_SEND,
				MODE_RECV,
				MODE_FINISH,
			};
			MODE mode_;
			msgpack::sbuffer	sbuf_;				// ���M�o�b�t�@
			msgpack::sbuffer	rbuf_;				// ��M�o�b�t�@
			HttpObject()
				: mode_(MODE_UNKNOWN)
			{}
		public:
			// ���http�I�u�W�F�N�g�œ�������N�G���͂ЂƂ����B�Ȃ��Ȃ猋�ʂƂЂ��Â���n���h�����K�v������
			bool Request(char* _http, char* _query, bool _isBlock = true)
			{
				if(MODE_UNKNOWN != mode_) return false;
				if(false == Open(_http,80)) return false;
				if(false == Connect(_isBlock)) return false;
				
				// ������e���o�b�t�@��
				sbuf_.clear();
				sbuf_.write(_query,strlen(_query));
				mode_ = MODE_INIT;
				if(_isBlock)
				{
					// ��������
					sendDatafromBuffer();
					recvDatafromBuffer();
				}

				return true;

			}
			bool sendDatafromBuffer()
			{
				if(MODE_INIT != mode_) return false;
				SendData(sbuf_.data(), sbuf_.size());
				mode_ = MODE_SEND;
				return true;
			}
			bool recvDatafromBuffer()
			{
				if(MODE_SEND != mode_) return false;
				if(false == RecvData(rbuf_, 0))
				{
					NET_TRACE("recvDataChankbuffer error:%s",std::string(rbuf_.data(),rbuf_.size()).c_str());
					return false;
				}

				NET_TRACE("recvDataChankbuffer:%s",std::string(rbuf_.data(),rbuf_.size()).c_str());
				mode_ = MODE_RECV;
				return true;
			}
			std::string GetResult()
			{
				if(MODE_RECV == mode_) mode_ = MODE_FINISH;
				return std::string(rbuf_.data(),rbuf_.size());
			}
			bool Complete()
			{
				return (MODE_RECV == mode_);
			}


		};

	public:
		HttpObjectWPtr GetHttpObject()
		{
			HttpObjectRPtr httpObject = new HttpObject();
			if(NULL != httpObject)
			{
				httpObject_list_.push_back(httpObject);
			}
			return httpObject;
		}

		typedef std::vector<NetObjectRPtr> NetObjectList;
		typedef std::vector<HttpObjectRPtr> HttpObjectList;
		NetObjectList netObject_list_;			// �i���I�Ȑڑ�
		HttpObjectList httpObject_list_;		// http�p��TCP�R�l�N�V�������X�g
		TCPObjectRPtr listener_;

		NetObjectRPtr netObject_me_;
	private:
		Network()
		{
			Initialize();
		}
		virtual ~Network(){}
	public:
		bool Initialize()
		{
			//	winsock������
			WSAData wsaData;
			if( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) )
			{
				NET_TRACE( "WSAStartup : %s\n", WSAGetLastErrorMessage() );
				return false;
			}

			// �����͓��ʂɒǉ�����
			netObject_me_ = new NetObject();
			netObject_me_->Open(GetMyAddr(), NET_CONNECT_PORT);
			netObject_me_->Connect(NULL);

			listener_ = new TCPObject();
			listener_->Open( INADDR_ANY, NET_CONNECT_PORT );
			listener_->Bind();
			listener_->Listen();

			return true;
		}

		bool Update()
		{
			fd_set readset_, writeset_, exset_;

			FD_ZERO(&readset_);
			FD_ZERO(&writeset_);
			FD_ZERO(&exset_);

			FD_SET(listener_->Socket(), &readset_);

			// tcp
			for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
			{
				if(INVALID_SOCKET != (*ite)->tcp_->Socket())
				{
					FD_SET((*ite)->tcp_->Socket(), &readset_);
					FD_SET((*ite)->tcp_->Socket(), &writeset_);
				}
			}
			
			// http
			for(HttpObjectList::iterator ite = httpObject_list_.begin(); httpObject_list_.end()!=ite; ite++)
			{
				if(INVALID_SOCKET != (*ite)->Socket())
				{
					if(HttpObject::MODE_INIT == (*ite)->mode_)
						FD_SET((*ite)->Socket(), &writeset_);
					if(HttpObject::MODE_SEND == (*ite)->mode_)
						FD_SET((*ite)->Socket(), &readset_);
				}
			}


			struct timeval to = {0,1};
			int n = select(0, &readset_, &writeset_, &exset_, &to);

			if (FD_ISSET(listener_->Socket(), &readset_)) {
				struct sockaddr_in in_addr;
				int in_addr_len = sizeof(in_addr);
				SOCKET new_sock = listener_->Accept(in_addr, in_addr_len);
				if (INVALID_SOCKET != new_sock) 
				{
					// accept
					RegistClient(in_addr.sin_addr.s_addr, ntohs(in_addr.sin_port),new_sock);
				}
			}
			// ����M
			// tcp
			for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
			{
				TCPObject& tcp = *static_cast<TCPObject*>((*ite)->tcp_);
				if(INVALID_SOCKET != tcp.Socket())
				{
					int r = 0;
					if (FD_ISSET(tcp.Socket(), &readset_)) {
						(*ite)->recvDatafromChankbuffer();
					}
					if (FD_ISSET(tcp.Socket(), &writeset_)) {
						(*ite)->sendDatafromChankbuffer();
					}

				}
			}
			// http
			for(HttpObjectList::iterator ite = httpObject_list_.begin(); httpObject_list_.end()!=ite; )
			{
				if(INVALID_SOCKET != (*ite)->Socket())
				{
					int r = 0;
					if (FD_ISSET((*ite)->Socket(), &readset_)) {
						(*ite)->recvDatafromBuffer();
					}
					if (FD_ISSET((*ite)->Socket(), &writeset_)) {
						(*ite)->sendDatafromBuffer();
					}
				}
				if(HttpObject::MODE_FINISH == (*ite)->mode_)
				{
					ite = httpObject_list_.erase(ite);
				}else{
					ite++;
				}

			}

			return true;
		}


		NetObjectRPtr RegistClient(const unsigned long _ipAddr, const unsigned short _port, SOCKET _socket = NULL)
		{
			if(NULL == netObject_me_) return NULL;

			NetObjectRPtr netObject = new NetObject();
			if(NULL == netObject) return NULL;

			// �R�l�N�g����
			if(NULL == _socket)
			{
				if(false == netObject->Open(_ipAddr, _port)) return NULL;
				if(false == netObject->Connect(netObject_me_)) return NULL;
			}else{
				if(false == netObject->Open(_ipAddr, _port, _socket)) return NULL;
			}
			netObject_list_.push_back(netObject);
			return netObject;
		}

		NetObjectRPtr RegistClient(const char* _ipAddr, const unsigned short _port)
		{
			if(NULL == netObject_me_) return NULL;

			// �ڑ���̃I�u�W�F�N�g�𐶐�
			NetObjectRPtr netObject = new NetObject();
			if(NULL == netObject) return NULL;

			// �R�l�N�g����
			if(false == netObject->Open(_ipAddr, _port)) return NULL;
			if(false == netObject->Connect(netObject_me_)) return NULL;
			netObject_list_.push_back(netObject);
			return netObject;
		}


		bool SendData( NetObjectRPtr rpToClient, const char* command, const msgpack::sbuffer& sbuf, NET_SEND_FLAG flag = NET_SEND_RELIABLE)
		{
			if(NULL == rpToClient)
			{
				// �S�����M
				for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
				{
					bool result = SendData((*ite),command, sbuf, flag );
					if(true != result)
						return result;
				}
				return true;
			}

			// �ʏ푗�M
			return  rpToClient->pushDatatoChankbuffer( command,  sbuf,  flag);
		}

		unsigned long GetMyAddr()
		{
			// ������IP�A�h���X�𓾂�
			char name[256];
			int re = gethostname(name, sizeof(name));
			if( 0 != re ) return 0;

			PHOSTENT hostinfo = gethostbyname(name);
			if( NULL == hostinfo ) return 0;
			return *(unsigned long*)((struct in_addr *)*hostinfo->h_addr_list);
		}


	};


}

