/*
 * ServerManager.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#include "ServerManager.h"

namespace npl {

ServerManager::ServerManager() {
	server = new Server(this);
	openedDispatcher = new OpenedDispatcher(this);
	lockedDispatcher = new LockedDispatcher();

}

void ServerManager::movePeerToOD(TCPSocket* peer) {
	openedDispatcher->addPeer(peer);
}

void ServerManager::movePeerToLD(char* user, TCPSocket* peer){
	lockedDispatcher->addPeer(user, peer);
}

void ServerManager::turnOnServer() {

}

ServerManager::~ServerManager() {
	delete server;
	delete openedDispatcher;
	delete lockedDispatcher;
}

} /* namespace npl */
