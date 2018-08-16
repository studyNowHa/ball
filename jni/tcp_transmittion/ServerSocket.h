// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"


class ServerSocket : public Socket
{
 public:

  ServerSocket ( int port );
  ServerSocket (){}
  virtual ~ServerSocket();

  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;
  void accept ( ServerSocket& );

  int RecvBuf(char *buf) const;

  void sendFile(std::string filepath, std::string filename/*, FILE *fp*/);
  int sendData(char buf[], int len);

  void closeSocket();



};


#endif
