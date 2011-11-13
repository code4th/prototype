#pragma once

#include <msgpack.hpp>
#include "Socket/SocketObject.h"
#include <string>
namespace kickflip
{
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
			, tcp_rcv_(NULL)
			, tcp_snd_(NULL)
			, udp_(NULL)
			, isLocal_(false)
			, isHost_(false)
		{}
		virtual ~NetObject() {}

	private:

		unsigned long		ipAddr_;			// IPアドレス
		unsigned short		port_;				// 通信ポート
		TCPObjectRPtr		tcp_rcv_;		// TCP通信受信用ソケット 
		TCPObjectRPtr		tcp_snd_;		// TCP通信送信用ソケット
		UDPObjectRPtr		udp_;			// UDP通信用ソケット
		bool				isLocal_;			// ローカル(自分)か
		bool				isHost_;			// ホスト(hosting)か これは、内部だけの情報で外部はこれを知る必要は無い

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

		ChankList sendChankReliableList_;		// 信頼性あり
		ChankList sendChankNonreliableList_;	// 信頼性なし
		ChankList recvChankList;				// 通常の受信バッファ

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
			//	winsock初期化
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
				// 全員送信
				for(NetClinetList::iterator ite = netclient_list_.begin(); netclient_list_.end()!=ite; ite++)
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

			// サーバからのHTTPメッセージ受信
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

				// 受信結果を表示
				//			fwrite(buf, n, 1, stdout);
			}

			return result;

		}

	};


}

