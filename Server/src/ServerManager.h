/*
 * ServerManager.h
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#ifndef SRC_SERVERMANAGER_H_
#define SRC_SERVERMANAGER_H_
#include "TCPSocket.h"
#include "OpenedDispatcher.h"
#include "LockedDispatcher.h"
#include "Server.h"
#include <iostream>
#include <string>

namespace npl {

class ServerManager: public ServerHandler, ODispatcherHandler {
	Server* server;
	OpenedDispatcher* openedDispatcher;
	LockedDispatcher* lockedDispatcher;

public:
	ServerManager();
	void movePeerToOD(TCPSocket* peer);
	void movePeerToLD(char* user, TCPSocket* peer);
	void turnOnServer();
	virtual ~ServerManager();
};

} /* namespace npl */

#endif /* SRC_SERVERMANAGER_H_ */
