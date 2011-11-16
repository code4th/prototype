#pragma once

#include <winsock2.h>
#include "../SmartPointer.h"
#include <stdio.h>
#include <tchar.h>
#include <msgpack.hpp>

namespace kickflip
{

	const char* WSAGetLastErrorMessage(const char* pcMessagePrefix="", int nErrorID = 0);

//	#define NET_PRINT( ... )			printf( __VA_ARGS__ )

#define NET_PRINT( str, ... ) \
	{ \
		TCHAR c[2048*10]; \
		_stprintf_s( c, str, __VA_ARGS__ ); \
		OutputDebugString( c ); \
	}	


	#define NET_TRACE( ... )		{ static unsigned int cnt = 0; NET_PRINT( "NetTrace(%d) %s(%d): ", ++cnt, __FILE__,__LINE__);  NET_PRINT( __VA_ARGS__ ); NET_PRINT( "\n" ); }
	#define NET_TRACE_THIS( ... )	{ static unsigned int cnt = 0; NET_PRINT( "NetTrace(%d) %s(%d): (0x%x) : ", ++cnt, __FILE__,__LINE__, this );  NET_PRINT( __VA_ARGS__ ); NET_PRINT( "\n" ); }
//	#define NET_TRACE( ... )		do { } while ( false )
//	#define NET_TRACE_THIS( ... )	do { } while ( false )

	SmartPtr(SocketObject);
	class SocketObject : public ReferenceObject
	{
	public:
		SocketObject()
			: socket_(INVALID_SOCKET)
		{}
		virtual ~SocketObject() {}
		virtual void Close()
		{
			closesocket( socket_ );
			socket_ = INVALID_SOCKET;
		}

		bool IsEnable(){ return INVALID_SOCKET!=socket_;}
		bool SetBlock(bool _isBlock)
		{
			unsigned long val=1;
			if(true == _isBlock) val = 0;
			ioctlsocket( socket_, FIONBIO, &val );

			return true;
		}
		bool NonDelay()
		{
			int flag = 1;
			int result = setsockopt(socket_,            /* 影響するソケット */
				IPPROTO_TCP,     /* TCP レベルのオプション設定 */
				TCP_NODELAY,     /* オプションの名前 */
				(char *) &flag,  /* このキャストは歴史的な汚点 */
				sizeof(int));    /* オプション値の長さ */
			if (result < 0){
				NET_TRACE( "NetObjectTCP TCP_NODELAY failed" );
				return false;
			}
			return true;
		}
		const SOCKET Socket() { return socket_;}
		const struct sockaddr_in& Sockaddr() { return sockaddr_in_;}


	protected:
		SOCKET	socket_;
		struct sockaddr_in sockaddr_in_;
		unsigned int **addrptr_;

	};

	SmartPtr(TCPObject);
	class TCPObject : public SocketObject
	{
	public:
		TCPObject()
			: SocketObject()
		{}
		virtual ~TCPObject() {Close();}

	public:
		bool Open( unsigned long _addr, unsigned short _port, SOCKET _socket = NULL );
		bool Open( const char* _addr, unsigned short _port );
		bool Bind();
		bool Listen(int _max_connect = 5);
		SOCKET Accept(struct sockaddr_in& _in_addr, int& _in_addr_len);

		bool Connect(bool _isBlock = true);
		virtual void Close();
		int SendData( const char* _data, unsigned int _dataSize);
		bool RecvData( msgpack::sbuffer& _buf, int _flags);
	};

	SmartPtr(UDPObject);
	class UDPObject : public SocketObject
	{
	public:
		UDPObject()
			: SocketObject()
		{}
		virtual ~UDPObject() {Close();}

	public:
		bool Open(unsigned short _port);
		virtual void Close();
		int SendData( const char* _data, unsigned int _dataSize, unsigned long _toAddr=0, unsigned short _toPort=0 );
		int RecvData( char* _data, unsigned int _dataSize, int _flags, struct sockaddr_in* _from=NULL, int* _fromlen=NULL );
	};



}

