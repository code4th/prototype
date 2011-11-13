#pragma once

#include <msgpack.hpp>
#include "Socket/SocketObject.h"
#include <string>
namespace kickflip
{
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
			, tcp_rcv_(NULL)
			, tcp_snd_(NULL)
			, udp_(NULL)
			, isLocal_(false)
			, isHost_(false)
		{}
		virtual ~NetObject() {}

	private:

		unsigned long		ipAddr_;			// IP�A�h���X
		unsigned short		port_;				// �ʐM�|�[�g
		TCPObjectRPtr		tcp_rcv_;		// TCP�ʐM��M�p�\�P�b�g 
		TCPObjectRPtr		tcp_snd_;		// TCP�ʐM���M�p�\�P�b�g
		UDPObjectRPtr		udp_;			// UDP�ʐM�p�\�P�b�g
		bool				isLocal_;			// ���[�J��(����)��
		bool				isHost_;			// �z�X�g(hosting)�� ����́A���������̏��ŊO���͂����m��K�v�͖���

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

		ChankList sendChankReliableList_;		// �M��������
		ChankList sendChankNonreliableList_;	// �M�����Ȃ�
		ChankList recvChankList;				// �ʏ�̎�M�o�b�t�@

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
		typedef std::vector<NetObjectRPtr> NetClinetList;
		NetClinetList netclient_list_;
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
				NET_TRACE( "WSAStartup : %d\n", WSAGetLastError() );
				return false;
			}
			return true;
		}
		void Finalize()
		{
			WSACleanup();
		}
		bool SendData( NetObjectRPtr& rpToClient, const char* command, const msgpack::sbuffer& sbuf, NET_SEND_FLAG flag = NET_SEND_RELIABLE)
		{
			if(NULL == rpToClient)
			{
				// �S�����M
				for(NetClinetList::iterator ite = netclient_list_.begin(); netclient_list_.end()!=ite; ite++)
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
		std::string HttpRequest(char* _http, char* _query)
		{
			std::string result("unKnown");
			TCPObjectRPtr tcpObject = new TCPObject();
			if(false == tcpObject->Open(_http,80)) return result;
			int n = tcpObject->SendData(_query,strlen(_query));

			if (n < 0) {
				NET_TRACE("HttpRequest: send err %s\n",_query);
				return result;
			}

			// �T�[�o�����HTTP���b�Z�[�W��M
			result.clear();
			while (n > 0) {
				char buf[64];
				memset(buf, 0, sizeof(buf));
				n = tcpObject->RecvData(buf, sizeof(buf)-1,0);
				if (n < 0) {
					NET_TRACE("HttpRequest: recv err %d\n", WSAGetLastError());
					return result;
				}
				buf[n] = '\0';
				result+=buf;

				// ��M���ʂ�\��
				//			fwrite(buf, n, 1, stdout);
			}

			return result;

		}

	};


}

