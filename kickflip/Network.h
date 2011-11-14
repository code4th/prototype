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
				sbuf_ = _sbuf;
			}
			virtual ~ChankData(){}
		private:
			ChankData(){}
		private:
			std::string command_;
			msgpack::sbuffer sbuf_;
		};
		typedef std::vector<ChankDataRPtr> ChankList;

		ChankList sendChankReliableList_;		// ���M�o�b�t�@�M��������
		ChankList sendChankNonreliableList_;	// ���M�o�b�t�@�M�����Ȃ�
		ChankList recvChankList_;				// ��M�o�b�t�@

	public:
		

	private:
		friend class Network;
		bool pushData( const char* _command, const msgpack::sbuffer& _sbuf, NET_SEND_FLAG _flag )
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
		bool sendDataNow( const char* _command, const msgpack::sbuffer& _sbuf, NET_SEND_FLAG _flag )
		{

		}
		bool sendDataTCP( const char* _command, const msgpack::sbuffer& _sbuf, NET_SEND_FLAG _flag )
		{
//			TCPSndObject_->SendData();
		}

	public:
		bool IsLocal() {return isLocal_;}
		const std::string& GetName(){ return name_;}

		bool Open( unsigned short _port )
		{
			// �܂�������NetObject���쐬
			unsigned long ipAddr = 0;
			{
				// ������IP�A�h���X�𓾂�
				char name[256];
				int re = gethostname(name, sizeof(name));
				if( 0 != re ) return false;

				PHOSTENT hostinfo = gethostbyname(name);
				if( NULL == hostinfo ) return false;
				ipAddr = *(unsigned long*)((struct in_addr *)*hostinfo->h_addr_list);
			}

			ipAddr_ = ipAddr;
			port_ = _port;

			name_ = (inet_ntoa( *(struct in_addr*)&ipAddr_ ));

			isLocal_ = true;

			//	TCP�̏������͂���Ȃ�

			//	UDP�p������
				// �������g�p��UDP�\�P�b�g�̍쐬
			udp_ = new UDPObject();
			if(NULL == udp_) return false;

			if( false == udp_->Open( port_ ) )
			{
				NET_TRACE( "UDPSocket Initialization failed!\n" );
				return false;
			}

			return true;

		}
		bool Connect(NetObjectRPtr connectNetObject, unsigned long _ipAddr, unsigned short _port)
		{
			ipAddr_ = _ipAddr;
			port_ = _port;

			name_ = (inet_ntoa( *(struct in_addr*)&ipAddr_ ));

			isLocal_ = false;

#if 0
			// ��M�\�P�b�g
			/* ��M�\�P�b�g��Accepter��������\�P�b�g���g������
			�C���X�^���X����������Ă���SetSocket�ɂđ�������*/
			tcp_rcv_ = new TCPObject();
			if(NULL == tcp_rcv_) return false;
			tcp_rcv_->Open();


			// ���M�\�P�b�g
			/* ���M�\�P�b�g�͎���̃^�C�~���O��Connect���č쐬����׏��������Ă���*/
			tcp_snd_  = new TCPObject();
			if(NULL == tcp_snd_) return false;
			if( false == tcp_snd_->Open( INADDR_ANY, port_ ) )
			{
				NET_TRACE( "TCPSrvSocket Initialization failed!\n" );
				return false;
			}
#endif
			// �ʐM�����UDP�\�P�b�g�͎����̃��m�����ʂŎg��
			udp_ = connectNetObject->udp_;
		}
	};


	SmartPtr(Network);
	class Network :public ReferenceObject
	{
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
			msgpack::sbuffer	sbuf_;				// ��M�o�b�t�@
		public:
			bool Request(char* _http, char* _query)
			{
				if(false == Open(_http,80)) return false;
				if(false == Connect()) return false;
				int n = SendData(_query,strlen(_query));

				if (n < 0) {
					NET_TRACE("HttpRequest: send err %s\n",_query);
					return false;
				}

				return true;

			}
			std::string GetResult()
			{
				// �T�[�o�����HTTP���b�Z�[�W��M
				sbuf_.clear();
				int n = 1;
				while (n > 0) {
					char buf[64];
					memset(buf, 0, sizeof(buf));
					n = RecvData(buf, sizeof(buf)-1,0);
					if (n < 0) {
						NET_TRACE("HttpRequest: recv err %s\n", WSAGetLastErrorMessage());
						return "";
					}
//					result+=buf;
					sbuf_.write(buf,n);
				}
				return std::string(sbuf_.data(),sbuf_.size());

			}

		};
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
		virtual ~Network()
		{
			Finalize();
		}
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

			netObject_me_->Open(NET_CONNECT_PORT);

			listener_ = new TCPObject();
			listener_->Open( INADDR_ANY, NET_CONNECT_PORT );
			listener_->Bind();
			listener_->Listen();

			return true;
		}

		void Finalize()
		{
			WSACleanup();
		}
		bool Update()
		{
			fd_set readset_, writeset_, exset_;

			FD_ZERO(&readset_);
			FD_ZERO(&writeset_);
			FD_ZERO(&exset_);

			FD_SET(listener_->Socket(), &readset_);

			for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
			{
				if(INVALID_SOCKET != (*ite)->tcp_->Socket())
				{
					FD_SET((*ite)->tcp_->Socket(), &readset_);
					FD_SET((*ite)->tcp_->Socket(), &writeset_);
				}
			}
			struct timeval to = {0,1};
			int n = select(0, &readset_, &writeset_, &exset_, &to);

			if (FD_ISSET(listener_->Socket(), &readset_)) {
				SOCKET new_sock = listener_->Accept();
				if (INVALID_SOCKET != new_sock) 
				{
					char text[] = "OLBAID_SERVER\nWelcome\n172.18.5.57\nIPList\n172.18.5.57\n";
					send(new_sock,text,sizeof(text),0);
/*
					make_nonblocking(fd);
					state[fd] = alloc_fd_state();
					assert(state[fd]);
*/
				}
			}

			for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
			{
				TCPObject& tcp = *(TCPObject*)((*ite)->tcp_);
				if(INVALID_SOCKET != tcp.Socket())
				{
					int r = 0;
					if (FD_ISSET(tcp.Socket(), &readset_)) {
//						r = tcp.RecvData();
					}
					if (FD_ISSET(tcp.Socket(), &writeset_)) {
//						r = tcp.SendData();
					}

				}
			}

			return true;
		}


		NetObjectRPtr RegistClient(const unsigned long _ipAddr, const unsigned short _port)
		{
			if(NULL == netObject_me_) return NULL;
			for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
			{
				if(_ipAddr == (*ite)->ipAddr_) return (*ite);
			}
			NetObjectRPtr netObject = new NetObject();
			if(NULL == netObject) return NULL;

			// �R�l�N�g����
			if(false == netObject->Connect(netObject_me_, _ipAddr, _port)) return NULL;
			netObject_list_.push_back(netObject);
			return netObject;
		}


		bool SendData( NetObjectRPtr& rpToClient, const char* command, const msgpack::sbuffer& sbuf, NET_SEND_FLAG flag = NET_SEND_RELIABLE)
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
			return  rpToClient->pushData( command,  sbuf,  flag);
		}

	public:

		HttpObjectRPtr GetHttpObject()
		{
			HttpObjectRPtr httpObject = new HttpObject();
			if(NULL != httpObject)
			{
				httpObject_list_.push_back(httpObject);
			}

			return httpObject;
		}


	};


}

