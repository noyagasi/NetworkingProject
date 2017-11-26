/*
 * Server.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#include "Server.h"
#include "TCPMessengerProtocol.h"


namespace npl {

Server::Server(ServerHandler* handler) {
	server = new TCPSocket(MSNGR_PORT);
	this->handler = handler;
	isAlive = true;
	start();
}

void Server::run() {
	while (isAlive == true) {
		TCPSocket* peerSocket = server->listenAndAccept();
		cout << "New TCP Socket " << peerSocket->peerAddrToString()
				<< " has been connected to the server" << "\n";

		handler->movePeerToOD(peerSocket);
	}
}

void Server::turnOn() {
}

void Server::shutDown() {
}



Server::~Server() {
	delete server;
	//delete handler;
}

} /* namespace npl */

