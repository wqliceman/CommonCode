// TCPSocket.cpp: implementation of the CTCPSocket class.  
//  
//////////////////////////////////////////////////////////////////////  

#include "TCPSocket.h"  

#pragma comment(lib, "ws2_32.lib")

class sockets_startupdown
{
public:
	sockets_startupdown();
	~sockets_startupdown() { WSACleanup( ); }

};
sockets_startupdown::sockets_startupdown (
	)
{
	WSADATA wsaData;
	WSAStartup (MAKEWORD(2,2), &wsaData);
}

void sockets_startup()
{
	static sockets_startupdown a;
}

//////////////////////////////////////////////////////////////////////  
CTCPSocket::CTCPSocket(SOCKET s)
{
	sockets_startup();
	m_socket = s;
}

CTCPSocket::CTCPSocket()
{
	sockets_startup();
	m_socket = INVALID_SOCKET;
}

CTCPSocket::~CTCPSocket()
{
}

int CTCPSocket::Create()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	return m_socket;
}

int CTCPSocket::Bind(u_short port /* = DEFAULT_PORT */)
{
	m_local.sin_addr.s_addr = htonl(INADDR_ANY);
	m_local.sin_family = AF_INET;
	m_local.sin_port = htons(port);

	int flag_value = 1;
	setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(&flag_value),sizeof(int));
	return bind(m_socket, (struct sockaddr*)&m_local, sizeof(m_local));
}

int CTCPSocket::Listen(int backlog)
{
	return listen(m_socket, backlog);
}

SOCKET CTCPSocket::Accept(sockaddr* addr /* = NULL */, int* addrlen /* = NULL */, unsigned long timeout)
{
	if (timeout>0)
	{
		fd_set read_set;
		FD_ZERO(&read_set);
		FD_SET(m_socket, &read_set);

		// setup a timeval structure
		timeval time_to_wait;
		time_to_wait.tv_sec = static_cast<long>(timeout/1000);
		time_to_wait.tv_usec = static_cast<long>((timeout%1000)*1000);

		int status = select(0,&read_set,0,0,&time_to_wait);

		if (status == SOCKET_ERROR || status == 0)
			return INVALID_SOCKET;
	}
	return accept(m_socket, addr, addrlen);
}

int CTCPSocket::Connect(DWORD ip, u_short port /* = DEFAULT_PORT */)
{
	m_foreign.sin_addr.s_addr = ip;
	m_foreign.sin_family = AF_INET;
	m_foreign.sin_port = htons(port);

	return connect(m_socket, (struct sockaddr*)&m_foreign, sizeof(m_foreign));
}

int CTCPSocket::Send(const char *pBuf, long num)
{
	const long old_num = num;
	long status;
	const long max_send_length = 1024*1024*100;
	while (num > 0)
	{
		const long length = min(max_send_length, num);
		if ( (status = send(m_socket, pBuf,length,0)) == SOCKET_ERROR)
		{
		}
		num -= status;
		pBuf += status;
	} 
	return old_num;
}

int CTCPSocket::Recv(char *pbuf, long num)
{
	const long max_recv_length = 1024*1024*100;
	const long length = min(max_recv_length, num);
	long status = recv(m_socket, pbuf,length,0);
	if (status == SOCKET_ERROR)
	{
		return 0;
	}
	return status;
}

int CTCPSocket::Recv(char* pbuf, long num, unsigned long timeout)
{
	if (Readable(timeout) == false)
		return -1;

	const long max_recv_length = 1024*1024*100;
	const long length = min(max_recv_length, num);
	long status = recv(m_socket,pbuf,length,0);
	if (status == SOCKET_ERROR)
	{
		return 0;
	}
	return status;
}

int  CTCPSocket::Close()
{
	int ret;

	ret = shutdown(m_socket, 2);
	return closesocket(m_socket);
}

int CTCPSocket::SetReceiveTimeOut(DWORD dwTimeout)
{
	int TimeOut = dwTimeout;
	return setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));
}

int CTCPSocket::SetSendTimeOut(DWORD dwTimeout)
{
	int TimeOut = dwTimeout;
	return setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&TimeOut, sizeof(TimeOut));
}

bool CTCPSocket::Readable(unsigned long timeout) const
{
	fd_set read_set;
	FD_ZERO(&read_set);

	FD_SET(m_socket, &read_set);

	timeval time_to_wait;
	time_to_wait.tv_sec = static_cast<long>(timeout/1000);
	time_to_wait.tv_usec = static_cast<long>((timeout%1000)*1000);

	// wait on select
	int status = select(0,&read_set,0,0,&time_to_wait);

	if (status <= 0)
		return false;
	return true;
}
