/*
 * Server.h
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_
#include "MThread.h"
#include "File.h"
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "File.h"
#include "iostream"
using namespace std;

namespace npl {
class ServerHandler {
public:
	virtual void movePeerToOD(TCPSocket* peer)=0;
};
class Server :public MThread{

	TCPSocket* server;
	ServerHandler* handler;
	bool isAlive;
	//File usersFile;

public:
	Server(ServerHandler* handler);
	void run();
	void turnOn();
	void shutDown();

	virtual ~Server();
};



} /* namespace npl */

#endif /* SRC_SERVER_H_ */
