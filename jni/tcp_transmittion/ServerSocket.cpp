// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include "utils/Log.h"

ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }

}

ServerSocket::~ServerSocket()
{
}


const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  
  if ( ! Socket::send ( s ) )
  {
    throw SocketException ( "Could not write to socket." );
  }

  return *this;
}


const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}

void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
  }
}

int ServerSocket::RecvBuf(char *buf) const
{
    int t = Socket::recvBuf(buf);
    return t;
}

void ServerSocket::sendFile(std::string filepath, std::string filename/*,FILE *fp*/)
{
    Socket::sendFile_(filepath, filename/*, fp*/);
}

int ServerSocket::sendData(char buf[] ,int len)
{
    return Socket::sendData_(buf, len);
}

void ServerSocket::closeSocket()
{
	Socket::closeSockId();
}

