/*! \file datagramsocket.h
 *   This is the header file where is defined the class DatagramSocket
 */
#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef _DATAGRAM_SOCKET_H
#define _DATAGRAM_SOCKET_H

#ifdef WIN32
 #include <winsock2.h>
 #pragma comment(lib,"ws2_32.lib")
#else
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <fcntl.h>

 #ifndef INVALID_SOCKET
  #define INVALID_SOCKET -1
 #endif
 #ifndef SOCKET_ERROR
  #define SOCKET_ERROR   -1
 #endif

#endif

#include "lintowin.h"

#define BUFFER_LEN 500
#define BUFFER_REAL_LEN 500+sizeof(short)

#define READY_TO_SEND 0x01
#define READY_TO_RECV 0x02

#define SERR_LEN 50

class DatagramSocket {
  private:
    int anyError;
    char strError[SERR_LEN];
    char socketReady;
    char buffer[BUFFER_REAL_LEN];
    struct sockaddr_in sockServer; /*server address*/
    struct sockaddr_in sockClient;
    
    static short numOfSockets;

    fd_set  readSet;
    timeval timeVal;

    short init();
    void end();

  public:

    DatagramSocket(unsigned short port, const char *ipAddr);
    DatagramSocket(unsigned short port, unsigned long nonBlockingMode = 1);
    virtual ~DatagramSocket();

    inline void setTimeVal(long sec, long usec) {
        timeVal.tv_sec = sec;
        timeVal.tv_usec = usec;
    }

    bool send(void *data, short len);
    bool receive(void *data, short *len);
#ifdef WIN32
    inline void close() { closesocket(m_Socket); }
#else
    inline void close() { ::close(m_Socket); }
#endif

    inline int getError() { return anyError; }
    inline char* getStrError() { return strError; }

  protected:
#ifdef WIN32
    SOCKET m_Socket;
#else
    int m_Socket;
#endif
};

#endif

