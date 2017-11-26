/*
 * LockedDispatcher.cpp
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#include "LockedDispatcher.h"
#include "TCPMessengerProtocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
namespace npl {

LockedDispatcher::LockedDispatcher() {
	isAlive = true;
	start();
}

void LockedDispatcher::updateListener() {
	vector<TCPSocket*> vector;
	for (map<string, TCPSocket *>::iterator it = this->onlinePeers.begin();
			it != this->onlinePeers.end(); ++it) {
		vector.push_back(it->second);
	}
	this->listener.add(vector);
}

int LockedDispatcher::sendCommand(TCPSocket* socket, const char* content,
		int command) {
	int commandNew = htonl(command);
	int result = socket->send((char*) &commandNew, 4);
	if (result < 0) {
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

LockedDispatcher::~LockedDispatcher() {
	isAlive = false;
}

void LockedDispatcher::pushAllScores(map<string, int> namesAndScores) {
	//Guard guard(&mutex2);
	ofstream ofs;
	ofs.open("scores.txt", ios::out | ofstream::trunc);
	if (ofs.is_open()) {
		ofs.clear();
		for (map<string, int>::const_iterator it = namesAndScores.begin();
				it != namesAndScores.end(); ++it) {
			ostringstream line;
			line << it->first << "::" << it->second << " " << "\n";
			ofs.write(line.str().data(), line.str().length());
		}
	}
	ofs.flush();
	ofs.close();
}

map<string, int> LockedDispatcher::getAllScores() {
	//Guard guard(&mutex2);
	map<string, int> allScores;
	fstream fs;
	fs.open("scores.txt");
	char line[32] = { '\0' };
	if (fs.is_open()) {
		fs.clear();
		fs.seekg(0, ios::beg);
		while (fs.getline(line, 32)) {
			//cout << line;
			int i = 0;
			string name;
			string score;
			while (line[i] != ':') {
				name += line[i];
				i++;
			}
			i += 2;
			while (line[i] != ' ') {
				score += line[i];
				i++;
			}
			istringstream buffer(score);
			int scoreByInt;
			buffer >> scoreByInt;
			allScores[name] = scoreByInt;
		}
		fs.flush();
		fs.close();
	}
	return allScores;
}

//Adding peer to the listener and onlinePeers
void LockedDispatcher::addPeer(char* user, TCPSocket* peer) {
	Guard guard(&mutex);
	cout << "New peer added to the ClosedDispatcher: "
			<< peer->peerAddrToString() << endl;
	int cnt = 0;
	char* temp = user;
	while (*user != ':') {
		cnt++;
		user++;
	}
	user = temp;
	string userstr = string(user);
	string name = userstr.substr(0, cnt);
	newName=name;
	if (onlinePeers.find(name) == onlinePeers.end()) {
		listener.add(peer);
		onlinePeers[name] = peer;
	}
}

void LockedDispatcher::close() {
	isAlive = false;
}

void LockedDispatcher::run() {

	while (isAlive) {
		currSock = listener.listen();
		if (currSock == NULL) {
			continue;
		}
		int command;
		currSock->recv((char*) &command, 4);
		command = htonl(command);
		switch (command) {

		case SHOW_USER_LIST: {
			cout << "SHOW_USER_LIST" << endl;
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char username[size + 1];
			currSock->recv(username, size);
			char name[size + 1];
			strncpy(name, username, size + 1);
			name[size] = '\0';
			char* temponlineusers = new char[120]();
			int cnt = 0;
			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				if (((it->first).compare(name)) != 0) {
					strcat(temponlineusers, (it->first).c_str());
					strcat(temponlineusers, "    ");
					cnt = cnt + 4 + (it->first).length();

				}
			}
			char* onlineusers = new char[cnt]();
			strcat(onlineusers, temponlineusers);
			sendCommand(currSock, onlineusers, SHOW_USER_LIST);
			break;
		}
		case EXIT: {
			//onlinePeers.erase(onlinePeers.find(currSock->peerAddrToString()));
			onlinePeers.erase(onlinePeers.find(newName));

			sleep(1);
			cout<<"deleting closed!"<<endl;
			//updating listener every time we delete sockets:
			updateListener();
			sleep(1);
			break;
		}
		case SEND_REQUEST: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char username[size + 1];
			currSock->recv(username, size);
			username[size] = '\0';
			char name[size + 1];
			name[0] = 0;
			if (strcmp(username, "empty") == 0) {
				for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
						it != onlinePeers.end(); ++it) {
					if (((it->second) != currSock)
							&& (inGamePlayers.find(it->first)
									== inGamePlayers.end())) {
						strncpy(name, (it->first).data(), size + 1);
						break;
					}
				}
			} else {
				strncpy(name, username, size + 1);
			}
			bool flag = false;
			if (name[0] == 0) {
				cout << "REjected" << endl;
				sendCommand(currSock, NULL,
				REJECT_REQUEST);
			} else {
				name[size] = '\0';
				for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
						it != onlinePeers.end(); ++it) {
					if ((it->second) == currSock) {
						sendCommand(onlinePeers[name], (it->first).c_str(),
						SEND_REQUEST);
						flag = true;
						break;
					}
				}
			}
			if (!flag)
				cout << "ld:SEND_REQUEST" << endl;
			break;
		}
		case ACCEPT_REQUEST: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char username[size + 1];
			currSock->recv(username, size);
			char name[size + 1];
			strncpy(name, username, size + 1);
			name[size] = '\0';
			bool flag = false;
			//cout << currSock->peerAddrToString() << endl;

			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				//cout << (it->second)->peerAddrToString() << endl;
				if ((it->second) == currSock) {
					char port1[5];
					char port2[5];
					sprintf(port1, "%d", currSock->getLocalPort());
					sprintf(port2, "%d", onlinePeers[name]->getLocalPort());
					sendCommand(onlinePeers[name], port1, START_GAME);
					inGamePlayers.insert(name);
					sendCommand(currSock, port2, START_GAME);
					inGamePlayers.insert(it->first);
					flag = true;
					break;
				}
			}

			if (!flag)
				cout << "ld:ACCEPT_REQUEST failed" << endl;
			break;
		}
		case REJECT_REQUEST: {
			int size;
			currSock->recv((char*) &size, 4);
			size = htonl(size);
			char username[size + 1];
			currSock->recv(username, size);
			char name[size + 1];
			strncpy(name, username, size + 1);
			name[size] = '\0';
			bool flag = false;
			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				if ((it->second) == currSock) {
					sendCommand(onlinePeers[name], (it->first).c_str(),
					REJECT_REQUEST);
					flag = true;
					break;
				}
			}
			if (!flag)
				cout << "ld:REJECT_REQUEST" << endl;
			break;
		}
		case WON: {
			//cout << "case WON" << endl;
			namesAndScores = getAllScores();

			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				if ((it->second) == currSock) {
					inGamePlayers.erase(it->first);
					if (namesAndScores[it->first] != NULL) {
						namesAndScores[it->first]++;
					} else {
						namesAndScores[it->first] = 1;
					}
					pushAllScores(namesAndScores);
					break;
				}
			}
			break;
		}
		case LOST: {
			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				if ((it->second) == currSock) {
					inGamePlayers.erase(it->first);
					break;
				}
			}
			break;
		}
		case REQUEST_SCOREBOARD: {
			namesAndScores = getAllScores();
			vector<pair<int, string> > newSocres;
			char clientName[] = "";
			for (map<string, TCPSocket*>::iterator it = onlinePeers.begin();
					it != onlinePeers.end(); ++it) {
				if ((it->second) == currSock) {
					strcpy(clientName, (it->first).c_str());

					break;
				}
			}

			for (map<string, int>::const_iterator it = namesAndScores.begin();
					it != namesAndScores.end(); ++it) {
				std::pair<int, string> thePair;
				thePair.first = it->second;
				thePair.second = it->first;
				newSocres.push_back(thePair);
			}
			cout << endl;
			sort(newSocres.begin(), newSocres.end());
			string content = "ALL SCORES:";
			content += "\n";
			for (int i = newSocres.size() - 1; i >= 0; i--) {
				content += newSocres[i].second;
				content += " ";
				content += newSocres[i].first + 48;
				content += "\n";
			}
			content += "Your rank is: ";

			for (int i = 0; i < newSocres.size(); i++) {
				if ((newSocres[i].second).compare(clientName) == 0) {
					content += (newSocres.size() - i) + 48;
					break;
				}
			}
			content += "\n";
			sendCommand(currSock, content.c_str(), PRINT_SCOREBOARD);
			break;
		}
		default: {
			cout << command << " Not a valid command! Try Again!" << endl;
		}
		}
	}
}

} /* namespace npl */
