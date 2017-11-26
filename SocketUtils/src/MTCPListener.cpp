/*
 * MTCPListener.cpp
 *
 *  Created on: Mar 20, 2016
 *      Author: user
 */

#include "MTCPListener.h"

namespace npl {

MTCPListener::MTCPListener() {
	sockets.clear();
}


void MTCPListener::add(TCPSocket* socket) {
	sockets.push_back(socket);
}

void MTCPListener::add(vector<TCPSocket*> sockets) {
	this->sockets = sockets;
}

TCPSocket* MTCPListener::listen() {
	struct timeval tv = { 1, 0 };
	tSockets::iterator iter = sockets.begin();
	tSockets::iterator endIter = sockets.end();
	fd_set fdset;
	FD_ZERO(&fdset);
	int highfd = 0;
	//fill the set with file descriptors
	for (; iter != endIter; iter++) {
		highfd++;
		FD_SET((*iter)->getSockFD(), &fdset);
		//if ((*iter)->getSocketFid()>highfd) highfd = (*iter)->getSocketFid();
	}

	//perform the select
	int returned = select(sizeof(fdset) * 8, &fdset, NULL, NULL, &tv);
	if (returned) {
		for (int i = 0; i < highfd; i++) {
			TCPSocket* tmpSocket = sockets[i];
			if (FD_ISSET(tmpSocket->getSockFD(), &fdset)) {
				return tmpSocket;
			}
		}
	}
	return NULL;
}

} /* namespace npl */

