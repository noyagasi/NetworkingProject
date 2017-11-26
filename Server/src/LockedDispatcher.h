/*
 * LockedDispatcher.h
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#ifndef SRC_LOCKEDDISPATCHER_H_
#define SRC_LOCKEDDISPATCHER_H_
#include <map>
#include <list>
#include <set>
#include <cassert>
#include <string>
#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Socket.h"
#include "TCPSocket.h"
#include "MThread.h"
#include "MTCPListener.h"
#include "Guard.h"
#include "TCPMessengerProtocol.h"

using namespace std;

namespace npl {

class LockedDispatcher : public MThread{

	MTCPListener listener;
	map<string, TCPSocket*> onlinePeers;
	set<string> inGamePlayers;
	map<string, int> namesAndScores;
	pthread_mutex_t mutex;
	pthread_mutex_t mutex2;
	TCPSocket* currSock;
	bool isAlive;
	string newName;
	void updateListener();
	int sendCommand(TCPSocket* socket, const char* content, int command);
public:
	LockedDispatcher();
	~LockedDispatcher();
	map<string, int> getAllScores();
	void pushAllScores(map<string, int> namesAndScores);
	void addPeer(char* user, TCPSocket* peer);
	void close();
	void run();
};

} /* namespace npl */


#endif /* SRC_LOCKEDDISPATCHER_H_ */
