// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>

#define    MAXLINE        1024

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  void closeSockId();

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  int recv ( std::string& ) const;
  int recvBuf(char *buf) const;

  //file Transimission
  void sendFile_(std::string filepath, std::string filename/*, FILE *fp*/);
  int sendData_(char buf[], int len);


  void set_non_blocking ( const bool );
  bool is_valid() const { return m_sock != -1; }
  int  get_sock_fd() const{ return m_sock; }

 private:
  int m_sock;
  sockaddr_in m_addr;

  //FILE  *fp;
  char  buf[MAXLINE];
  std::string fileName_send;
  int fileSize_send;
  int sendSize;

  bool isconnected;
  bool isPacketHead;
};


#endif

