/*
 * OpenedDispa#include "TCPMessengerProtocol.h"
 tcher.cpp
 *
 *  Created on: Aug 12, 2017
 *      Author: user
 */

#include "OpenedDispatcher.h"
#include "TCPMessengerProtocol.h"
#include <fstream>

namespace npl {

OpenedDispatcher::OpenedDispatcher(ODispatcherHandler* handler) {
	this->handler = handler;
	isAlive = true;
	start();
}

bool OpenedDispatcher::saveUser(char* user) {
	Guard guard(&mutex1);
	ofstream ofs;
	ofs.open("server.txt", ios::out | ios::app);
	ofs.clear();
	ofs << user;
	ofs << endl;
	ofs.flush();
	ofs.close();
	return true;
}
bool OpenedDispatcher::doLogin(char* user, TCPSocket* sock){
	handler->movePeerToLD(user, sock);
	onlinePeers.erase(onlinePeers.find(sock->peerAddrToString()));
	updateListener();
	return true;
}

bool OpenedDispatcher::verifyUser(char* user) {
	Guard guard(&mutex1);
	char line[256] = { '\0' };
	ifstream ifs;

	ifs.open("server.txt", ifstream::in);
	if (ifs.is_open()) {

		ifs.clear();
		ifs.seekg(0, ios::beg);
		while (ifs.getline(line, 256)) {
			string newline = string(line);
			if (newline.compare(user) == 0) {
				ifs.close();
				return true;
			}
		}
	} else {
		cout << "file not found " << endl;
	}
	return false;
}

void OpenedDispatcher::updateListener() {
	vector<TCPSocket*> vector;
	for (map<string, TCPSocket *>::iterator it = this->onlinePeers.begin();
			it != this->onlinePeers.end(); ++it) {
		vector.push_back(it->second);
	}
	this->listener.add(vector);
}

bool OpenedDispatcher::checkName(char* userName) {
	Guard guard(&mutex1);
	char line[256] = { '\0' };
//char* line=NULL;
	ifstream ifs;
	ifs.open("server.txt", ifstream::in);
	if (ifs.is_open()) {
		ifs.clear();
		ifs.seekg(0, ios::beg);
		while (ifs.getline(line, 256)) {
			string newline = string(line);
			string str = newline.substr(0, strlen(userName) - 1);
			string newUser = string(userName);
			string blabla = newUser.substr(0, strlen(newUser.data()) - 1);
			// if(strcmp(line,userName)==0){
			if (str.compare(blabla) == 0) {
				//cout << "username is exist " << endl;
				ifs.close();
				return true;
			}
		}
	} else {
		cout << "file not found " << endl;
	}
	return false;
}

void OpenedDispatcher::run() {
	while (isAlive) {
		currSock = listener.listen();
		if (currSock == NULL) {
			continue;
		}
		int command;

		currSock->recv((char*) &command, 4);
		command = htonl(command);
		switch (command) {
		case CHECK_USERNAME: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char name[size + 1];
			currSock->recv(name, size);
			if (!checkName(name)) { //name is ok
				sendCommand(currSock, NULL, AVAILABLE_USERNAME);
			} else { //taken
				sendCommand(currSock, NULL, TAKEN_USERNAME);
			}
			break;
		}
		case REGISTER: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char userDetails[size + 1];
			currSock->recv(userDetails, size);
			int index;
			for (index = 0; index < static_cast<int>(strlen(userDetails));
					index++) {
				if (userDetails[index] <= 'z' && userDetails[index] >= '0')
					;
				//	cout << userDetails[index] << endl;
				else
					break;
			}
			index++;
			char user[index];
			strncpy(user, userDetails, index - 1);
			user[index - 1] = '\0';
			//cout << user << endl;
			if (saveUser(user)) {
				sendCommand(currSock, NULL, REGISTER_SUCCESSFUL);
			} else {
				sendCommand(currSock, NULL, REGISTER_FAIL);
			}
			break;
		}
		case LOGIN: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char userDetails[size + 1];
			currSock->recv(userDetails, size);
			int index;
			for (index = 0; index < static_cast<int>(strlen(userDetails));
					index++) {
				if (!(userDetails[index] <= 'z' && userDetails[index] >= '0'))
					break;
			}
			index++;
			char user[index];
			strncpy(user, userDetails, index - 1);
			user[index - 1] = '\0';
			if (verifyUser(user)) {
				if (doLogin(user, currSock))
					sendCommand(currSock, NULL, LOGIN_SUCCESSFUL);
			} else {
				sendCommand(currSock, NULL, LOGIN_FAIL);
			}
			break;
		}

		case EXIT: {
			onlinePeers.erase(onlinePeers.find(currSock->peerAddrToString()));
			sleep(1);
			cout<<"deleting open"<<endl;
			//updating listener every time we delete sockets:
			updateListener();
			sleep(1);
			break;
		}
		default: {
			cout << "Not a valid command! Try Again!" << endl;
			break;
		}
		}
	}
}

int OpenedDispatcher::sendCommand(TCPSocket* socket, const char* content,
		int command) {
	int commandNew = htonl(command);
	int result = socket->send((char*) &commandNew, 4);
	if (result < 0) {
		//cout << "LockedDispatcher::sendCommand::result" << endl;
		return -1;
	}
	if (content != NULL) {
		int len = strlen(content);
		int lenNew = htonl(len);
		result = socket->send((char*) &lenNew, 4);
		if (result < 0)
			return -1;
		result = socket->send(content, len);
		if (result < 0)
			return -1;
	}
	return result;
}

void OpenedDispatcher::addPeer(TCPSocket* peer) {
	Guard guard(&mutex2);
	string peerAddressAndPort = peer->peerAddrToString();
	cout << "New peer added to the OpenedDispatcher: " << peerAddressAndPort
			<< endl;
//Checking if the peer is already connected
	if (onlinePeers.find(peerAddressAndPort) == onlinePeers.end()) {
		listener.add(peer);
		onlinePeers[peerAddressAndPort] = peer;
	}
}

OpenedDispatcher::~OpenedDispatcher() {
	//file->closeSock();
}

}

