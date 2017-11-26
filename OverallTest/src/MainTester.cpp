/*
 * MainTester.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#include "MainTester.h"

bool MainTester::test() {

	//creating the server
	ServerManager server = ServerManager();
	sleep(1);

	//creating two clients
	Client client1 = Client(this);
	Client client2 = Client(this);

	//connecting the clients to the server's ip
	client1.connect("127.0.0.1");
	client2.connect("127.0.0.1");
	sleep(1);

	///hashing the password of client1
	unsigned hPass = client1.hashPass("1234");
	sleep(1);
	//checks if client registered
	if (lastCmd == REGISTER_SUCCESSFUL) {
		cout << "REGISTER_SUCCESSFUL" << endl;
	}

	//client1 logging in with username and hashed password
	client1.LogInUser((char*) "Omri", hPass);
	sleep(1);

	//checks if the user logged in successfully
	if (lastCmd == LOGIN_SUCCESSFUL) {
		cout << "Hello Omri!" << endl;
	}

	//client2 logs in with username and hashed password
	client2.LogInUser((char*) "Noy", hPass);
	sleep(1);

	//checks if the user logged in successfully
	if (lastCmd == LOGIN_SUCCESSFUL) {
		cout << "Hello Noy!" << endl;
	}
	sleep(1);

	//printing the score board
	client1.printScoreBoard();

	//client2 asks to show all the online players
	client2.getOnlinePlayers((char*) "Noy");
	sleep(1);

	//prints all the online players
	cout << lastMsg << endl;
	int i = 0;
	while (lastMsg[i] != 32) { //space
		i++;
	}

	//gets the random player's name and asks to start a game with him
	string randomPlayer = lastMsg.substr(0, i);
	client2.sendRequest((char*) (randomPlayer.c_str()));
	sleep(1);

	//the other player accepts the request
	client1.acceptRequest();

	//the game itself happens here, all UDP messages exchange
	sleep(3000);

	//after the game is finished and the time ends the clients disconnect from the server
	//and exit the server
	client1.disconnect();
	client1.exit();
	client2.disconnect();
	client2.exit();

	//server shut down
	sleep(1);
	server.~ServerManager();

	return true;
}



