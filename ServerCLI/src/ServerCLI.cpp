/*
 * ServerCLI.cpp
 *
 *  Created on: Aug 27, 2017
 *      Author: user
 */

#include "ServerCLI.h"

void ServerCLI::runCLI() {
	cout << "Welcome to the game's Server" << endl;
	ServerManager server = ServerManager();
			sleep(2000);
	char input;
	/*printMenu();
	cin >> input;
	switch (input) {
	case 's': {
		ServerManager server = ServerManager();
		sleep(2000);
		break;
	}
	case 'x': {
		return;
	}
	}*/
}

void ServerCLI::printMenu() {
	cout << "Please pick one of the options below : " << endl;
	cout << "To turn on the server press -  s" << endl;
	cout << "To exit press - x" << endl;
}
