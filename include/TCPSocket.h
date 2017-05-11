// TCPSocket.h: interface for the CTCPSocket class.  
//  
//////////////////////////////////////////////////////////////////////  

#if !defined(_TCPSOCKET_H_)  
#define _TCPSOCKET_H_  

#include "winsock2.h"  

class CTCPSocket
{
public:
	CTCPSocket();
	CTCPSocket(SOCKET s);
	virtual ~CTCPSocket();

	int Create();
	int Bind(u_short port = DEFAULT_PORT);
	int Listen(int backlog = DEFAULT_BACKLOG);
	SOCKET Accept(sockaddr* addr = NULL, int* addrlen = NULL, unsigned long timeout = 0);

	int Connect(DWORD ip, u_short port = DEFAULT_PORT);
	int Send(const char *pBuf, long len);
	int Recv(char *pbuf, long len);
	int Recv(char* pbuf, long len, unsigned long timeout);
	int SetReceiveTimeOut(DWORD dwTimeout);
	int SetSendTimeOut(DWORD dwTimeout);

	int Close();

	SOCKET m_socket;
private:

	bool Readable(unsigned long timeout)const;

	sockaddr_in m_local;
	sockaddr_in m_foreign;

	enum { DEFAULT_PORT = 56177, DEFAULT_BACKLOG = 5 };
};

#endif //_TCPSOCKET_H_  