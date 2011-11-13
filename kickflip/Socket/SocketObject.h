#pragma once

#include <winsock2.h>
#include "../SmartPointer.h"
#include <stdio.h>

namespace kickflip
{
	#define NET_PRINT( ... )			printf( __VA_ARGS__ )

	#define NET_TRACE( ... )		{ static unsigned int cnt = 0; NET_PRINT( "NetTrace(%d) : ", ++cnt );  NET_PRINT( __VA_ARGS__ ); NET_PRINT( "\n" ); }
	#define NET_TRACE_THIS( ... )	{ static unsigned int cnt = 0; NET_PRINT( "NetTrace(%d) : (0x%x) : ", ++cnt, this );  NET_PRINT( __VA_ARGS__ ); NET_PRINT( "\n" ); }
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

	protected:
		SOCKET	socket_;
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
		bool Open( unsigned long addr, unsigned short port );
		bool Open( const char* addr, unsigned short port );
		virtual void Close();
		int SendData( const char* pData, unsigned int dataSize);
		int RecvData( char* pData, unsigned int dataSize, int flags);
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
		bool Open(unsigned short port);
		virtual void Close();
		int SendData( const char* pData, unsigned int dataSize, unsigned long toAddr=0, unsigned short toPort=0 );
		int RecvData( char* pData, unsigned int dataSize, int flags, struct sockaddr_in* from=NULL, int* fromlen=NULL );
	};



}

