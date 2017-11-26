/*
 * MainTester.h
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#ifndef MAINTESTER_H_
#define MAINTESTER_H_
#include "ServerManager.h"
#include "Client.h"

using namespace npl;
using namespace std;

class MainTester : public ClientHandler {
private:
	int lastCmd;
	string lastMsg;
public:
	void onCommandRecieved(int cmd){ lastCmd = cmd; }
	void onMessageRecieved(string message) { lastMsg = message; }
	void onOpenSession(string ip, int port) { lastMsg = "onOpenSession"; }
	void onClosedSession(){ lastMsg = "onClosedSession"; }
	void onOtherPlayerReq(char* username){ cout << username << " wants to play with you :)" << endl; }
	bool test();
	//~MainTester();
};

#endif /* MAINTESTER_H_ */
