#include "datagramsocket.h"
#include <cstring>

short DatagramSocket::numOfSockets = 0;

short DatagramSocket::init() 
{
  anyError=0;
#ifdef WIN32
  if (!numOfSockets) {
    WSADATA wsaData;
    if ( WSAStartup(0x0202,&wsaData) ) { /*windows socket startup */
      STRCPY(strError,SERR_LEN,"Could not start WSA");
      anyError=1;
    }
  }
#endif
  ++numOfSockets;
  return anyError;
}

void DatagramSocket::end()
{
  close();
#ifdef WIN32
  WSACleanup();
#endif
}

DatagramSocket::DatagramSocket(unsigned short port, const char *ipAddr) 
{
  socketReady = 0;
  if ( init() ) return;
  memset((char*)&sockClient,0,sizeof(sockClient));
  sockClient.sin_family = AF_INET; /* address protocol family*/
  sockClient.sin_port = htons(port); /*set port*/
  sockClient.sin_addr.s_addr = inet_addr(ipAddr);
  if (sockClient.sin_addr.s_addr == INADDR_NONE) {
    STRCPY(strError,SERR_LEN,"Bad ip address");
    anyError=2;
    return ;
  }

  m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (m_Socket == INVALID_SOCKET) {
    STRCPY(strError,SERR_LEN,"Invalid socket");
    anyError=3;
    return ;
  }

  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;

  socketReady = READY_TO_SEND ;
}

DatagramSocket::DatagramSocket(unsigned short port,
		               unsigned long nonBlockingMode) 
{
  socketReady = 0;
  if ( init() ) return;
  memset((char*)&sockServer,0,sizeof(sockServer));
  sockServer.sin_family = AF_INET; /* address protocol family*/
  sockServer.sin_addr.s_addr = INADDR_ANY;
  sockServer.sin_port = htons(port); /*set port*/

  m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (m_Socket == INVALID_SOCKET) {
    STRCPY(strError,SERR_LEN,"Invalid socket");
    anyError=3;
    return ;
  }

  if (bind( m_Socket,        /*bind a server address and port*/
       (struct sockaddr *) &sockServer, 
        sizeof(sockServer)) ) {
    close();
    STRCPY(strError,SERR_LEN,"bind() failed");
    anyError=4;
    return ;
  }

  if (nonBlockingMode) {
#ifdef WIN32
    anyError = ioctlsocket(m_Socket, FIONBIO, &nonBlockingMode);
#else
    anyError = fcntl(m_Socket, F_SETFL, O_NONBLOCK);
#endif
    if ( anyError ) {
      close();
      STRCPY(strError,SERR_LEN,"ioctlsocket() failed");
      return;
    }
  }

  timeVal.tv_sec = 0;
  timeVal.tv_usec = 0;

  socketReady = READY_TO_RECV;
}

DatagramSocket::~DatagramSocket() 
{
  --numOfSockets;
  if (!numOfSockets) end();
}

bool DatagramSocket::send(void *data, short len) 
{
  anyError=0;

  if (len < 1 && len > BUFFER_LEN ) {
    STRCPY(strError,SERR_LEN,"Incorrect value of len");
    anyError=5;
    return false;
  }
  if (! (socketReady & READY_TO_SEND) ) {
    STRCPY(strError,SERR_LEN,"Unknow destination address");
    anyError=6;
    return false;
  }

  char *pBuffer = buffer;

  memset(pBuffer,0,BUFFER_REAL_LEN);

  memcpy(pBuffer,&len, sizeof(short));
  pBuffer += sizeof(short);

  memcpy(pBuffer, data, len);

  anyError = sendto(m_Socket, buffer, len+sizeof(short), 0,
                     (struct sockaddr *)&sockClient, sizeof(sockClient));
  if (anyError == SOCKET_ERROR) {
    STRCPY(strError,SERR_LEN,"sento() error");
    anyError=7;
    return false;
  }

  socketReady |= READY_TO_RECV;
  return true;
}

bool DatagramSocket::receive(void *data, short *pLen) 
{
  char *pBuffer = buffer;
#ifdef WIN32
  int sockLen = sizeof(sockClient);
#else
  socklen_t sockLen = sizeof(sockClient);
#endif

  anyError=0;

  if (! (socketReady & READY_TO_RECV) ) {
    STRCPY(strError,SERR_LEN,"Socket is not ready to receive");
    anyError=8;
    return false;
  }

  // Check for messages
  FD_ZERO( &readSet );
  FD_SET( m_Socket, &readSet );

  anyError = select( m_Socket+1 , &readSet, NULL, NULL, &timeVal );

  // anyError == 0 -> There is no data to read in socket 
  if (!anyError) return false;

  if (anyError == SOCKET_ERROR) {
    STRCPY(strError,SERR_LEN,"select() error");
    anyError=9;
    return false;
  }

  memset(pBuffer,0,BUFFER_REAL_LEN);

  anyError = recvfrom(m_Socket,buffer,BUFFER_REAL_LEN,0,
                       (struct sockaddr *)&sockClient, &sockLen);
  if (anyError == SOCKET_ERROR) {
    STRCPY(strError,SERR_LEN,"redvfrom() error");
    anyError=10;
    return false;
  }

  memcpy(pLen, pBuffer, sizeof(short));
  pBuffer += sizeof(short);

  memcpy(data, pBuffer, *pLen);

  socketReady |= READY_TO_SEND ;

  return true;
}

