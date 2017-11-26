/*
 * ClientCLI.cpp
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#include "ClientCLI.h"

void ClientCLI::runCLI() {
	Client client = Client(this);
	client.connect("127.0.0.1");
	bool loggedIn = false;
	string userName = "";
	startGame = false;
	inGame = false;
	bool isPlaying = false;
	cout << "xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxo" << endl;
	cout << "x                                    o" << endl;
	cout << "x           Welcome to our           o" << endl;
	cout << "x          Tic Tac Toe Game          o" << endl;
	cout << "x                                    o" << endl;
	cout << "xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxo" << endl;
	cout << endl;
	while (!loggedIn) {
		cout << endl;
		cout << "Please pick one of the options below : " << endl;
		cout << "Register -                            	r" << endl;
		cout << "Login -                               	l" << endl;
		cout << "Exit-                               	x" << endl;

		char action;
		cin >> action;
		switch (action) {
		//register
		case 'r': {
			string password;
			unsigned hashedPass;
			do {
				cout << "Enter username: " << endl;
				cin >> userName;
				client.isNameTaken((char*) (userName.c_str()));
				sleep(1);
				if (lastCmd == TAKEN_USERNAME) {
					cout << userName
							<< " is already taken, please try something else."
							<< endl;
				}
			} while (lastCmd != AVAILABLE_USERNAME);
			cout << "Enter password: " << endl;
			cin >> password;
			hashedPass = client.hashPass(password.c_str());
			client.registerUser((char*) userName.c_str(), hashedPass);
			sleep(1);
			if (lastCmd == REGISTER_SUCCESSFUL) {
				cout << "Registration succeed, logging in...." << endl;
				client.LogInUser((char*) userName.c_str(), hashedPass);
				sleep(1);
				if (lastCmd == LOGIN_SUCCESSFUL) {
					cout << "You have entered the game lobby!" << endl;
					loggedIn = true;
				}
			}
			break;
		}
		case 'x':{
			client.disconnect();
			sleep(1);
			client.exit();
			break;
		}
			//login
		case 'l': {
			string password;
			unsigned hashedPass;
			do {
				cout << "Enter username: " << endl;
				cin >> userName;
				cout << "Enter password: " << endl;
				cin >> password;
				hashedPass = client.hashPass(password.c_str());
				client.LogInUser((char*) userName.c_str(), hashedPass);
				sleep(1);
				if (lastCmd == LOGIN_SUCCESSFUL) {
					cout << "You have entered the game lobby!" << endl;
					loggedIn = true;
				} else
					cout << "Login failed, try again." << endl;
			} while (lastCmd != LOGIN_SUCCESSFUL);
			break;
		}
		default:
			break;
		}
	}
	while (!isPlaying) {
		sleep(2);
		cout << endl;
		cout << "Please pick one of the options below : " << endl;
		cout << "Show online players -                 	s" << endl;
		cout << "Start a game with a specific player - 	g" << endl;
		cout << "Start a game with a random player -   	d" << endl;
		cout << "Exit-                               	x" << endl;

		char action;
		cin >> action;
		switch (action) {
		case 's': {
			client.getOnlinePlayers((char*) userName.c_str());
			sleep(1);
			break;
		}
			//start a game with a player
		case 'g': {
			string enemy;
			cout << "Enter the player's name you want to play with: " << endl;
			cin >> enemy;
			client.sendRequest((char*) (enemy.c_str()));
			cout << "Please wait for the other player..." << endl;
			while (!startGame) {
				if (lastCmd == REJECT_REQUEST) {
					cout << "The other player rejected you request" << endl;
					inGame = false;
					break;
				}
				inGame = true;
				sleep(1);
			}
			break;
		}
			//start a game with a random player
		case 'd': {
			client.sendRequest((char*) "empty");
			cout << "Please wait for the other player..." << endl;
			while (!startGame) {
				if (lastCmd == REJECT_REQUEST) {
					cout << "The other player rejected you request" << endl;
					inGame = false;
					break;
				}
				inGame = true;
				sleep(1);
			}
			break;
		}
		case 'a': {
			client.acceptRequest();
			inGame = true;
			break;
		}
		case 'r': {
			client.rejectRequest();
			break;
		}

		case 'x': {
			client.disconnect();
			sleep(1);
			client.exit();
			isPlaying=true;
			break;
		}
		default: {
			break;
		}
		}
		while (inGame) {
			sleep(1);
		}
	}
}

