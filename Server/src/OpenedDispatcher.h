/*
 * OpenedDispatcher.h
 *
 *  Created on: Aug 12, 2017
 *      Author: user
 */

#ifndef SRC_OPENEDDISPATCHER_H_
#define SRC_OPENEDDISPATCHER_H_

#include <MThread.h>
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "File.h"
#include "Guard.h"
#include "MTCPListener.h"
#include <map>
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

namespace npl {
class ODispatcherHandler {
public:
	virtual void movePeerToLD(char* user, TCPSocket* peer)=0;
};

class OpenedDispatcher: public MThread {
	ODispatcherHandler* handler;
	TCPSocket* currSock;
	bool isAlive;
	File* file;
	pthread_mutex_t mutex1, mutex2;
	MTCPListener listener;
	int sendCommand(TCPSocket* socket, const char* content, int command);
	void updateListener();
	map<string, TCPSocket*> onlinePeers;
	bool doLogin(char* user, TCPSocket* currSock);

public:
	OpenedDispatcher(ODispatcherHandler* handler);
	bool saveUser(char* user);
	bool verifyUser(char* user);
	bool checkName(char* userName);
	void run();
	void addPeer(TCPSocket* peer);
	virtual ~OpenedDispatcher();
};
}
#endif /* SRC_OPENEDDISPATCHER_H_ */
