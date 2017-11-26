/*
 * MTCPListener.h
 *
 *  Created on: Mar 20, 2016
 *      Author: user
 */

#ifndef MTCPLISTENER_H_
#define MTCPLISTENER_H_

#include "TCPSocket.h"
#include "iostream"
#include <vector>

using namespace std;
namespace npl {

class MTCPListener  {
	typedef vector<TCPSocket*> tSockets;
	vector<TCPSocket*> sockets;

public:
	/**
	 * add socket to listen to
	 */
	MTCPListener();
	void add(TCPSocket* socket);
	void add(vector<TCPSocket*> sockets);
	//void clear(){ sockets.clear(); }
	TCPSocket* listen();

};

} /* namespace npl */

#endif /* MTCPLISTENER_H_ */
