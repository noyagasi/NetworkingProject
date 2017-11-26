/*
 * ClientCLI.h
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#ifndef CLIENTCLI_H_
#define CLIENTCLI_H_

#include <iostream>
#include "Client.h"
#include <string>
#include "TCPMessengerProtocol.h"
#include "MThread.h"
using namespace npl;

using namespace std;

class ClientCLI : public ClientHandler {
private:
	string lastMsg;
	int lastCmd;
public:
	void onCommandRecieved(int cmd){ lastCmd = cmd; }
	void onMessageRecieved(string message) { lastMsg = message; }
	void onOpenSession(string ip, int port) { lastMsg = "onOpenSession"; }
	void onClosedSession(){ lastMsg = "onClosedSession"; }
	void onOtherPlayerReq(char* username){
		cout << username << " wants to play with you :)" << endl;
		cout << "Accept - a, Reject - r" << endl;	}

	void runCLI();
};
#endif /* CLIENTCLI_H_ */
