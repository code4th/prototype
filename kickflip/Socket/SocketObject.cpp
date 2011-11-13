

#include "SocketObject.h"


namespace kickflip
{

	/////////////////////////////////////////////
	/*TCP***************************************/
	/////////////////////////////////////////////
	// �ڑ�
	bool TCPObject::Open( unsigned long addr, unsigned short port )
	{
		Close();
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if( INVALID_SOCKET == socket_ ) return false;

		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons( port );
		server.sin_addr.S_un.S_addr = addr;

		if( 0 != connect( socket_, (struct sockaddr *)&server, sizeof( server ) )  )
		{
			if(WSAEISCONN == WSAGetLastError()){
				NET_TRACE( "NetObjectTCP(CLT) connected : %s\n", inet_ntoa( server.sin_addr ) );
				return true;
			}else{
				NET_TRACE( "NetObjectTCP connect : %d\n", WSAGetLastError() );
				Close();
				return false;
			}
		}

		NonDelay();

		NET_TRACE( "NetObjectTCP(CLT) connected : %s\n", inet_ntoa( server.sin_addr ) );

		return true;
	}
	bool TCPObject::Open( const char* addr, unsigned short port )
	{
		Close();
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if( INVALID_SOCKET == socket_ ) return false;

		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons( port );
		// �Ƃ肠�����A�A�h���X��ϊ����Ă݂�
		server.sin_addr.S_un.S_addr = inet_addr(addr);

		if (INADDR_NONE == server.sin_addr.S_un.S_addr) {
			// �ϊ��o���Ȃ����
			struct hostent *host = gethostbyname(addr);
			if ( NULL == host ) {
				if (WSAGetLastError() == WSAHOST_NOT_FOUND) {
					NET_TRACE( "NetObjectTCP host not found : %s\n", addr );
				}
				Close();
				return false;
			}

			unsigned int **addrptr = (unsigned int **)host->h_addr_list;
			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);

				if( 0 == connect( socket_, (struct sockaddr *)&server, sizeof( server ) )  )
				{
					goto next;
				}

				addrptr++;
				// connect�����s�����玟�̃A�h���X�Ŏ����܂�
			}

			// connect���S�Ď��s�����ꍇ
			if (*addrptr == NULL) {
				NET_TRACE( "NetObjectTCP can't connect All list: %d\n", WSAGetLastError() );
				Close();
				return false;
			}
		}


		if( 0 != connect( socket_, (struct sockaddr *)&server, sizeof( server ) )  )
		{
			NET_TRACE( "NetObjectTCP can't connect : %d\n", WSAGetLastError() );
			Close();
			return false;
		}

next:
		SetBlock(false);
		NonDelay();
		NET_TRACE( "NetObjectTCP(CLT) Address connected : %s\n", inet_ntoa( server.sin_addr ) );

		return true;
	}
	void TCPObject::Close()
	{
		if( INVALID_SOCKET == socket_ ) return;

		char	DataBuf[1024];
		//	��M��~��, ���ɑ��M�ς݃f�[�^���擾��, �\�P�b�g�����
		shutdown( socket_, SD_SEND );		//	��M����
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
			{	//	���MOK
				//printf( "send %d byes.\n", sendSize ) );
				totalSize += sendSize;
			}
			else
			{	//	�{�C(�܂�)�G���[
				//printf( "error : 0x%x\n", WSAGetLastError() );
				NET_TRACE( "NetObjectTCP SOCKET_ERROR : %d\n", WSAGetLastError() );
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
			{	//	��MOK
				//printf( "recvFrom %s\n", inet_ntoa( from.sin_addr ) );
				totalSize += rcvSize;
			}
			else
			{	//	�{�C(�܂�)�G���[
				NET_TRACE( "NetObjectTCP recv error : %d\n", WSAGetLastError() );
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
			NET_TRACE( "NetObjectUDP socket error: %d\n", WSAGetLastError() );
			return false;
		}

		struct sockaddr_in inaddr;
		inaddr.sin_family = AF_INET;
		inaddr.sin_port = htons( port );
		inaddr.sin_addr.S_un.S_addr = INADDR_ANY;

		if( bind( socket_, (struct sockaddr *)&inaddr, sizeof( inaddr ) ) )
		{
			NET_TRACE( "NetObjectUDP bind error: %d\n", WSAGetLastError() );
			Close();
			return false;
		}

		//	�m���u���b�L���O�ݒ�(val = 0�Ńu���b�L���O���[�h�ݒ�)
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
		{	//	��MOK
			//printf( "recvFrom %s\n", inet_ntoa( from->sin_addr ) );
		}
		else
		{	//	�{�C(�܂�)�G���[
			NET_TRACE( "NetObjectUDP recv error : %d\n", WSAGetLastError() );
		}

		return rcvSize;
	}

}