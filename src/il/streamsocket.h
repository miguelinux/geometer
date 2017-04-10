/*! \file streamsocket.h
 *  This is the header file where is defined the class StreamSocket
 */
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_STREAM_SOCKET_H_
#define IL_STREAM_SOCKET_H_

#ifdef WIN32
  #include <windows.h>
  #include <winsock2.h>
  #pragma comment(lib,"ws2_32.lib")
#else

  #include <netinet/in.h>

  #ifndef SOCKET
    #define SOCKET int
  #endif
  #ifndef INVALID_SOCKET  
    #define INVALID_SOCKET -1
  #endif
  #ifndef SOCKET_ERROR
    #define SOCKET_ERROR -1
  #endif
#endif

namespace il {

  class StreamSocket {
    private:
      int anyError;
      bool isBinded; /* true if is a server socket */
      bool isConnected; /* true if is a client socket */
      struct sockaddr_in service; /*server address*/
      static short numOfSockets;

      void init();
      void end();
  
    public:
      StreamSocket();
      StreamSocket(unsigned short port, const char *ipAddr);
      StreamSocket(unsigned short port, short backlog= 5,
		      const char *bindAddr = 0);
  
      virtual ~StreamSocket();
  
      void close();
      bool accept(StreamSocket &clientSocket);
      bool send(void *data, int len);
      int recive(void *data, int len, bool wait = false);
  
    protected:

      void setNoDelay(SOCKET s);
      void setNonBlocking(SOCKET s);
  
      StreamSocket(SOCKET s, struct sockaddr_in sAddr);
  
      SOCKET m_Socket;
  }; //class StreamSocket 
}; //namespace il

#endif

