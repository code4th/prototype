

#include "SocketObject.h"
//#include <iostream>
#include <algorithm>
#include <strstream>

namespace kickflip
{
	void NetTrace( LPCSTR pszFormat, ...)
	{
		va_list	argp;
		char pszBuf[256];
		va_start(argp, pszFormat);
		vsprintf_s( pszBuf, 256, pszFormat, argp);
		va_end(argp);
		OutputDebugString( pszBuf);
	}

	const char* WSAGetLastErrorMessage(const char* pcMessagePrefix, int nErrorID /* = 0 */)
	{

		//// WSAGetLastErrorMessage ////////////////////////////////////////////
		// A function similar in spirit to Unix's perror() that tacks a canned 
		// interpretation of the value of WSAGetLastError() onto the end of a
		// passed string, separated by a ": ".  Generally, you should implement
		// smarter error handling than this, but for default cases and simple
		// programs, this function is sufficient.
		//
		// This function returns a pointer to an internal static buffer, so you
		// must copy the data from this function before you call it again.  It
		// follows that this function is also not thread-safe.

		static struct ErrorEntry {
			int nID;
			const char* pcMessage;

			ErrorEntry(int id, const char* pc = 0)
				: nID(id)
				, pcMessage(pc) 
			{}

			bool operator<(const ErrorEntry& rhs) 
			{
				return nID < rhs.nID;
			}
		} gaErrorList[] = {
			ErrorEntry(0,                  "No error"),
			ErrorEntry(WSAEINTR,           "Interrupted system call"),
			ErrorEntry(WSAEBADF,           "Bad file number"),
			ErrorEntry(WSAEACCES,          "Permission denied"),
			ErrorEntry(WSAEFAULT,          "Bad address"),
			ErrorEntry(WSAEINVAL,          "Invalid argument"),
			ErrorEntry(WSAEMFILE,          "Too many open sockets"),
			ErrorEntry(WSAEWOULDBLOCK,     "Operation would block"),
			ErrorEntry(WSAEINPROGRESS,     "Operation now in progress"),
			ErrorEntry(WSAEALREADY,        "Operation already in progress"),
			ErrorEntry(WSAENOTSOCK,        "Socket operation on non-socket"),
			ErrorEntry(WSAEDESTADDRREQ,    "Destination address required"),
			ErrorEntry(WSAEMSGSIZE,        "Message too long"),
			ErrorEntry(WSAEPROTOTYPE,      "Protocol wrong type for socket"),
			ErrorEntry(WSAENOPROTOOPT,     "Bad protocol option"),
			ErrorEntry(WSAEPROTONOSUPPORT, "Protocol not supported"),
			ErrorEntry(WSAESOCKTNOSUPPORT, "Socket type not supported"),
			ErrorEntry(WSAEOPNOTSUPP,      "Operation not supported on socket"),
			ErrorEntry(WSAEPFNOSUPPORT,    "Protocol family not supported"),
			ErrorEntry(WSAEAFNOSUPPORT,    "Address family not supported"),
			ErrorEntry(WSAEADDRINUSE,      "Address already in use"),
			ErrorEntry(WSAEADDRNOTAVAIL,   "Can't assign requested address"),
			ErrorEntry(WSAENETDOWN,        "Network is down"),
			ErrorEntry(WSAENETUNREACH,     "Network is unreachable"),
			ErrorEntry(WSAENETRESET,       "Net connection reset"),
			ErrorEntry(WSAECONNABORTED,    "Software caused connection abort"),
			ErrorEntry(WSAECONNRESET,      "Connection reset by peer"),
			ErrorEntry(WSAENOBUFS,         "No buffer space available"),
			ErrorEntry(WSAEISCONN,         "Socket is already connected"),
			ErrorEntry(WSAENOTCONN,        "Socket is not connected"),
			ErrorEntry(WSAESHUTDOWN,       "Can't send after socket shutdown"),
			ErrorEntry(WSAETOOMANYREFS,    "Too many references, can't splice"),
			ErrorEntry(WSAETIMEDOUT,       "Connection timed out"),
			ErrorEntry(WSAECONNREFUSED,    "Connection refused"),
			ErrorEntry(WSAELOOP,           "Too many levels of symbolic links"),
			ErrorEntry(WSAENAMETOOLONG,    "File name too long"),
			ErrorEntry(WSAEHOSTDOWN,       "Host is down"),
			ErrorEntry(WSAEHOSTUNREACH,    "No route to host"),
			ErrorEntry(WSAENOTEMPTY,       "Directory not empty"),
			ErrorEntry(WSAEPROCLIM,        "Too many processes"),
			ErrorEntry(WSAEUSERS,          "Too many users"),
			ErrorEntry(WSAEDQUOT,          "Disc quota exceeded"),
			ErrorEntry(WSAESTALE,          "Stale NFS file handle"),
			ErrorEntry(WSAEREMOTE,         "Too many levels of remote in path"),
			ErrorEntry(WSASYSNOTREADY,     "Network system is unavailable"),
			ErrorEntry(WSAVERNOTSUPPORTED, "Winsock version out of range"),
			ErrorEntry(WSANOTINITIALISED,  "WSAStartup not yet called"),
			ErrorEntry(WSAEDISCON,         "Graceful shutdown in progress"),
			ErrorEntry(WSAHOST_NOT_FOUND,  "Host not found"),
			ErrorEntry(WSANO_DATA,         "No host data of that type was found")
		};
		const int kNumMessages = sizeof(gaErrorList) / sizeof(ErrorEntry);
		// List of Winsock error constants mapped to an interpretation string.
		// Note that this list must remain sorted by the error constants'
		// values, because we do a binary search on the list when looking up
		// items.

		// Build basic error string
		static char acErrorBuffer[256];
		std::ostrstream outs(acErrorBuffer, sizeof(acErrorBuffer));
		outs << pcMessagePrefix << ": ";

		// Tack appropriate canned message onto end of supplied message 
		// prefix. Note that we do a binary search here: gaErrorList must be
		// sorted by the error constant's value.
		ErrorEntry* pEnd = gaErrorList + kNumMessages;
		ErrorEntry Target(nErrorID ? nErrorID : WSAGetLastError());

		struct cmp
		{
			inline bool operator ()(const ErrorEntry& lhs, const ErrorEntry& rhs) const
			{
			  return lhs.nID < rhs.nID;
			}
		};
		ErrorEntry* it = std::lower_bound(gaErrorList, pEnd, Target,cmp());
		if ((it != pEnd) && (it->nID == Target.nID)) {
			outs << it->pcMessage;
		}
		else {
			// Didn't find error in list, so make up a generic one
			outs << "unknown error";
		}
		outs << " (" << Target.nID << ")";

		// Finish error message off and return it.
		outs << std::ends;
		acErrorBuffer[sizeof(acErrorBuffer) - 1] = '\0';
		return acErrorBuffer;
	}


	/////////////////////////////////////////////
	/*TCP***************************************/
	/////////////////////////////////////////////
	// 接続
	bool TCPObject::Open( unsigned long addr, unsigned short port )
	{
		Close();
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if( INVALID_SOCKET == socket_ ) return false;

		//memcpy( server_, 0, sizeof( server_ ) );

		server_.sin_family = AF_INET;
		server_.sin_port = htons( port );
		server_.sin_addr.S_un.S_addr = addr;

		return true;
	}

	bool TCPObject::Open( const char* addr, unsigned short port )
	{
		Open( static_cast< unsigned long>(0), port );
		// とりあえず、アドレスを変換してみる
		server_.sin_addr.S_un.S_addr = inet_addr(addr);

		if (INADDR_NONE == server_.sin_addr.S_un.S_addr) {
			// 変換出来なければ
			struct hostent *host = gethostbyname(addr);
			if ( NULL == host ) {
				if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
					NET_TRACE( "TCPObject host not found : %s\n", addr );
				}
				Close();
				return false;
			}

			addrptr_ = (unsigned int **)host->h_addr_list;
		}

		return true;
	}

	bool TCPObject::Connect()
	{
		if(NULL != addrptr_)
		{
			while (true) 
			{
				if (*addrptr_ == NULL) {
					NET_TRACE( "TCPObject can't connect All list: %s\n", WSAGetLastErrorMessage() );					
					Close();
					return false;
				}

				server_.sin_addr.S_un.S_addr = *(*addrptr_);
				if( 0 == connect( socket_, (struct sockaddr *)&server_, sizeof( server_ ) )  )
				{
					break;
				}

				addrptr_++;
			}
		}else{
			if( 0 != connect( socket_, (struct sockaddr *)&server_, sizeof( server_ ) )  )
			{
				if(WSAEISCONN != WSAGetLastError()){
					NET_TRACE( "TCPObject can't connect : %s\n", WSAGetLastErrorMessage() );
					Close();
					return false;
				}
			}
		}

		//SetBlock(false);
		NonDelay();
		NET_TRACE( "TCPObject Address connected : %s\n", inet_ntoa( server_.sin_addr ) );

		return true;
	}

	bool TCPObject::Bind()
	{
		if( 0 != bind( socket_, (struct sockaddr *)&server_, sizeof( server_ ) )  )
		{
			NET_TRACE( "TCPObject bind error: %s\n", WSAGetLastErrorMessage());
			return false;
		}
		return true;
	}

	bool TCPObject::Listen(int _max_connect)
	{
		if( 0 != listen( socket_, _max_connect ) )
		{
			NET_TRACE( "TCPObject listen(%D) error : %s\n", _max_connect, WSAGetLastErrorMessage());
			return false;
		}
		//SetBlock(false);
		return true;
	}

	SOCKET TCPObject::Accept()
	{
		struct sockaddr_in in_addr;
		int in_addr_len = sizeof(in_addr);

		SOCKET new_sock = accept( socket_, (struct sockaddr*)&in_addr, &in_addr_len );
		if( new_sock == INVALID_SOCKET || 0 > new_sock )
		{
			NET_TRACE( "TCPObject accept error: %s\n", WSAGetLastErrorMessage());
			return INVALID_SOCKET;
		}

		NET_TRACE( "TCPObject accept: %s(%d)\n", inet_ntoa(in_addr.sin_addr), ntohs(in_addr.sin_port) );

		return new_sock;
	}

	void TCPObject::Close()
	{
		addrptr_ = NULL;
		if( INVALID_SOCKET == socket_ ) return;

		char	DataBuf[1024];
		//	受信停止後, 既に送信済みデータを取得し, ソケットを閉じる
		shutdown( socket_, SD_SEND );		//	受信拒否
		while( recv( socket_, DataBuf, sizeof( DataBuf ), 0 ) > 0 );
		SocketObject::Close();
	}

	int TCPObject::SendData( const char* pData, unsigned int dataSize)
	{
		unsigned int totalSize = 0;

		while( totalSize < dataSize )
		{
			int sendSize = send( socket_, (char*)(pData + totalSize), dataSize - totalSize, 0 );

			if( sendSize != SOCKET_ERROR )
			{	//	送信OK
				//printf( "send %d byes.\n", sendSize ) );
				totalSize += sendSize;
			}
			else
			{	//	本気(まじ)エラー
				//printf( "error : 0x%x\n", WSAGetLastError() );
				NET_TRACE( "TCPObject SOCKET_ERROR : %s\n", WSAGetLastErrorMessage() );
				return -1;
			}
		}

		return totalSize;
	}
	int TCPObject::RecvData( char* pData, unsigned int dataSize, int flags)
	{
		unsigned int totalSize = 0;
		char *pDataBuf = (char *)malloc( dataSize );
		memset( pDataBuf, 0, dataSize );

		while( totalSize < dataSize )
		{
			int rcvSize = recv( socket_, pDataBuf + totalSize, dataSize - totalSize, flags );

			if( rcvSize == 0 )
			{
				break;
			}
			else if( SOCKET_ERROR != rcvSize )
			{	//	受信OK
				//printf( "recvFrom %s\n", inet_ntoa( from.sin_addr ) );
				totalSize += rcvSize;
			}
			else
			{	//	本気(まじ)エラー
				NET_TRACE( "TCPObject recv error : %s\n", WSAGetLastErrorMessage() );
				free( pDataBuf );
				return rcvSize;
			}
		}
		memcpy( pData, pDataBuf, totalSize );
		free( pDataBuf );

		return totalSize;
	}

	/////////////////////////////////////////////
	/*UDP***************************************/
	/////////////////////////////////////////////
	bool UDPObject::Open(unsigned short port)
	{
		Close();
		socket_ = socket( AF_INET, SOCK_DGRAM, 0 );
		if( INVALID_SOCKET == socket_ )
		{
			NET_TRACE( "UDPObject socket error: %s\n", WSAGetLastErrorMessage() );
			return false;
		}

		struct sockaddr_in inaddr;
		inaddr.sin_family = AF_INET;
		inaddr.sin_port = htons( port );
		inaddr.sin_addr.S_un.S_addr = INADDR_ANY;

		if( bind( socket_, (struct sockaddr *)&inaddr, sizeof( inaddr ) ) )
		{
			NET_TRACE( "UDPObject bind error: %s\n", WSAGetLastErrorMessage() );
			Close();
			return false;
		}

		//	ノンブロッキング設定(val = 0でブロッキングモード設定)
		unsigned long val=1;
		//	ioctlsocket(m_kSocket, FIONBIO, &val);

		return true;
	}
	void UDPObject::Close()
	{
		if( INVALID_SOCKET == socket_ ) return;

		SocketObject::Close();
	}
	int UDPObject::SendData( const char* pData, unsigned int dataSize, unsigned long toAddr, unsigned short toPort )
	{
		sockaddr_in	sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_addr.s_addr = toAddr;
		sockAddr.sin_port = htons( toPort );
		sockAddr.sin_family = AF_INET;

		return sendto( socket_, (char*)pData, dataSize, 0, (struct sockaddr *)&sockAddr, sizeof(sockAddr) );
	}
	int UDPObject::RecvData( char* pData, unsigned int dataSize, int flags, struct sockaddr_in* from, int* fromlen )
	{
		int rcvSize = recvfrom( socket_, pData, dataSize, flags, (struct sockaddr *)from, fromlen);

		if( SOCKET_ERROR != rcvSize )
		{	//	受信OK
			//printf( "recvFrom %s\n", inet_ntoa( from->sin_addr ) );
		}
		else
		{	//	本気(まじ)エラー
			NET_TRACE( "UDPObject recv error : %s\n", WSAGetLastErrorMessage() );
		}

		return rcvSize;
	}

}