#pragma once

#include <msgpack.hpp>
#include "Socket/SocketObject.h"
#include <string>
namespace kickflip
{
	#define		NET_CONNECT_PORT		(30002)

	static const char* header = "origin";
	// データ送信タイプ
	enum NET_SEND_FLAG{
		NET_SEND_NONRELIABLE		=	0x00000000,	// 高速で信頼性なし。パケットはロストする可能性がある
		NET_SEND_RELIABLE			=	0x00000001,	// 信頼性あり。必ず届くこと前提。
		NET_SEND_SYNC_BUCKET		=	0x00000004,	// bucketSyncに使用するイベント。これを使うとイベントが整った状態で処理することができる。
		NET_SEND_NOW				=	0x00000008,	// バッファにためずにすぐ送る
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

		unsigned long		ipAddr_;		// IPアドレス
		unsigned short		port_;			// 通信ポート
		TCPObjectRPtr		tcp_;			// TCP通信用ソケット
		UDPObjectRPtr		udp_;			// UDP通信用ソケット
		bool				isLocal_;		// ローカル(自分)か
		bool				isHost_;		// ホスト(hosting)か これは、内部だけの情報で外部はこれを知る必要は無い

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

		ChankList sendChankReliableList_;		// 送信バッファ信頼性あり
		ChankList sendChankNonreliableList_;	// 送信バッファ信頼性なし
		ChankList recvChankList_;				// 受信バッファ

	public:
		

	private:
		friend class Network;
		bool pushData( const char* _command, const msgpack::sbuffer& _sbuf, NET_SEND_FLAG _flag )
		{
			// 通常の送信はローカルにはしない
			if(true == IsLocal()) return true;

			ChankDataRPtr chankdata = new ChankData(_command, _sbuf);

			if(_flag & NET_SEND_RELIABLE)
			{
				// 信頼性通信
				sendChankReliableList_.push_back(chankdata);
			}else{
				// 高速通信
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
			// まず自分のNetObjectを作成
			unsigned long ipAddr = 0;
			{
				// 自分のIPアドレスを得る
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

			//	TCPの初期化はいらない

			//	UDP用初期化
				// 自分自身用のUDPソケットの作成
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
			// 受信ソケット
			/* 受信ソケットはAccepterから作られるソケットを使うため
			インスタンスだけを作っておきSetSocketにて代入される*/
			tcp_rcv_ = new TCPObject();
			if(NULL == tcp_rcv_) return false;
			tcp_rcv_->Open();


			// 送信ソケット
			/* 送信ソケットは自らのタイミングでConnectして作成する為初期化しておく*/
			tcp_snd_  = new TCPObject();
			if(NULL == tcp_snd_) return false;
			if( false == tcp_snd_->Open( INADDR_ANY, port_ ) )
			{
				NET_TRACE( "TCPSrvSocket Initialization failed!\n" );
				return false;
			}
#endif
			// 通信相手のUDPソケットは自分のモノを共通で使う
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
			msgpack::sbuffer	sbuf_;				// 受信バッファ
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
				// サーバからのHTTPメッセージ受信
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
		NetObjectList netObject_list_;			// 永続的な接続
		HttpObjectList httpObject_list_;		// http用のTCPコネクションリスト
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
			//	winsock初期化
			WSAData wsaData;
			if( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) )
			{
				NET_TRACE( "WSAStartup : %s\n", WSAGetLastErrorMessage() );
				return false;
			}

			// 自分は特別に追加する
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

			// コネクトする
			if(false == netObject->Connect(netObject_me_, _ipAddr, _port)) return NULL;
			netObject_list_.push_back(netObject);
			return netObject;
		}


		bool SendData( NetObjectRPtr& rpToClient, const char* command, const msgpack::sbuffer& sbuf, NET_SEND_FLAG flag = NET_SEND_RELIABLE)
		{
			if(NULL == rpToClient)
			{
				// 全員送信
				for(NetObjectList::iterator ite = netObject_list_.begin(); netObject_list_.end()!=ite; ite++)
				{
					bool result = SendData((*ite),command, sbuf, flag );
					if(true != result)
						return result;
				}
				return true;
			}

			// 通常送信
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

