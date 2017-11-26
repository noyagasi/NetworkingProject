/*
 * TCPSocket.h
 *
 *  Created on: May 4, 2017
 *      Author: Omri Grossman
 */

#ifndef SRC_TCPSOCKET_H_
#define SRC_TCPSOCKET_H_


#include "Socket.h"
using namespace std;

namespace npl{
class TCPSocket : public Socket {
private:
	/**
	 *constructor to create a second TCP  socket to communicate with a client
	 */
	TCPSocket(int connected_sock, struct sockaddr_in serverAddr,
			struct sockaddr_in peerAddr);
public:

	/**
	 * constructor to create a TCP server socket
	 */
	TCPSocket(int port);
	/**
	 * constructor to creates TCP client socket
	 and connect it to the remote peer in the given ip and port.
	 */
	TCPSocket(string perrIP, int port);

	/**
	 *perform listen and accept on server socket,
	 this function returns a new TCPSocket object to communicate over the incoming
	 session.
	 */
	TCPSocket* listenAndAccept();
	/**
	 *read from socket into the given buffer up to
	 the buffer given length, return the number of bytes read.
	 */
	int recv(char* buff, int len);
	/**
	 * send the given buffer to the socket.
	 */
	int send(const char* msg, int len);
	/**
	 *close the socket and free all resources.
	 */
//	void closeSock();
	/**
	 * return the address of the connected peer.
	 */
	string fromAddr();
	//string fromAddr();
	/**
	 * returns socket_fd
	 */
	int socket();

	int getPort();

	string peerAddrToString();

	string localAddrToString();

};
}
#endif /* SRC_TCPSOCKET_H_ */
