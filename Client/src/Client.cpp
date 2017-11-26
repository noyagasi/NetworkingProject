/*
 * Client.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#include "Client.h"
#include "TCPMessengerProtocol.h"

using namespace std;
namespace npl {

Client::Client(ClientHandler* newHandler) {
	square = "0123456789";
	newHandler->startGame = false;
	mark = 'O';
	this->handler = newHandler;
	myTurn = false;
	tcpSocket = NULL;
	isPlaying = false;
	isConnected = false;
	wasMarked = true;
	userName = NULL;
	pass = NULL;
	hashedPass = NULL;
	udpSocket = NULL;
	peerPort = NULL;
}

Client::~Client() {
	if (tcpSocket != NULL) {
		delete tcpSocket;
	}
}

bool Client::connect(string ip) {
	tcpSocket = new TCPSocket(ip, MSNGR_PORT);
	isConnected = true;
	this->start();
	return true;
}


void Client::disconnect() {
	sendServerCommand(EXIT, NULL);
}

void Client::exit() {
	if (tcpSocket != NULL) {
		tcpSocket->closeSock();
		delete tcpSocket;

	}
}

int Client::getPort() {
	return tcpSocket->getPort();
}

int Client::sendServerCommand(int cmd, const char* buff) {

	int cmdNet = htonl(cmd);

	int res = tcpSocket->send((char*) &cmdNet, 4);
	if (res < 4) {
		return -1;
	}
	if (buff != NULL) {
		int len = strlen(buff);
		int lenNet = htonl(len);
		res = tcpSocket->send((char*) &lenNet, 4);
		if (res < 4) {
			return -1;
		}
		res = tcpSocket->send(buff, len);
		if (res < len) {
			return -1;
		}
	}
	return res;
}

void Client::recvClientCommand() {
	char newNum[10];
	memset((void*) newNum, 0, 10);
	//Guard guard(&mutex);
	udpSocket->recv(newNum, 4);
	char peerMark = 'N';
	if (mark == 'X')
		peerMark = 'O';
	else
		peerMark = 'X';
	updateTable(newNum, peerMark);
	board();
	if (checkWin() != 1) {
		makeMove();
	} else {
		cout << "Loser haha!" << endl;
		isPlaying = false;
		handler->inGame = false;
		sleep(1);
		sendServerCommand(LOST, NULL);
		sleep(1);
		printScoreBoard();
	}
}

int Client::recvServerCommand() {
	//if the client is not in a game, listen to the tcp socket:
	int cmd;
	int res = tcpSocket->recv((char*) &cmd, 4);
	//cout << "TCPMSNClient::recvCommand" << endl;
	cmd = ntohl(cmd);
	if (res < 4) {
		return -1;
	}
	switch (cmd) {
	case EXIT: {
		handler->onClosedSession();
		break;
	}
	case TAKEN_USERNAME: {
		handler->onCommandRecieved(TAKEN_USERNAME);
		break;
	}
	case AVAILABLE_USERNAME: {
		handler->onCommandRecieved(AVAILABLE_USERNAME);
		break;
	}
	case REGISTER_SUCCESSFUL: {
		handler->onCommandRecieved(REGISTER_SUCCESSFUL);
		break;
	}
	case REGISTER_FAIL: {
		handler->onCommandRecieved(REGISTER_FAIL);
		break;
	}
	case LOGIN_SUCCESSFUL: {
		handler->onCommandRecieved(LOGIN_SUCCESSFUL);
		break;
	}
	case LOGIN_FAIL: {
		handler->onCommandRecieved(LOGIN_FAIL);
		break;
	}
	case SHOW_USER_LIST: {
		int size;
		int res = tcpSocket->recv((char*) &size, 4);
		if (res < 4) {
			return -1;
		}
		size = htonl(size);
		char* buffer = new char[size];
		res = tcpSocket->recv(buffer, size);
		if (res < size) {
			return -1;
		}
		cout << buffer << endl;
		handler->onMessageRecieved(string(buffer));
		break;
	}
	case SEND_REQUEST: {
		int size;
		mark = 'X';
		int res = tcpSocket->recv((char*) &size, 4);
		if (res < 4) {
			return -1;
		}
		size = htonl(size);
		char* username = new char[size];
		res = tcpSocket->recv(username, size);
		if (res < size) {
			return -1;
		}
		handler->onOtherPlayerReq(username);
		wantsToPlayWithMe = username;
		break;
	}
	case START_GAME: {
		int size;
		int res = tcpSocket->recv((char*) &size, 4);
		if (res < 4) {
			return -1;
		}
		size = htonl(size);
		char* port = new char[size];
		res = tcpSocket->recv(port, size);
		if (res < size) {
			return -1;
		}
		string strport = string(port);
		int intport = 0;
		for (int i = 0; i < 5; i++) {
			intport = intport + ((strport[i]) - 48);
			intport *= 10; // casting between string to int
		}
		intport /= 10;
		peerPort = intport;
		if (udpSocket == NULL){
		udpSocket = new UDPSocket(this->getPort());
		}
		square = "0123456789";
		handler->onCommandRecieved(ACCEPT_REQUEST);
		handler->startGame = true;
		handler->inGame=true;
		isPlaying = true;
		sleep(1);
		break;
	}
	case REJECT_REQUEST: {
		handler->onCommandRecieved(REJECT_REQUEST);
		break;
	}
	case PRINT_SCOREBOARD: {
		int size;
		int res = tcpSocket->recv((char*) &size, 4);
		if (res < 4) {
			return -1;
		}
		size = htonl(size);
		char* buffer = new char[size];
		res = tcpSocket->recv(buffer, size);
		if (res < size) {
			return -1;
		}
		cout << string(buffer) << endl;
		break;
	}
	default:
		break;
	}

	return 1;
}

unsigned Client::hashPass(const char* s) {
	unsigned h = FIRSTH;
	while (*s) {
		h = (h * A) ^ (s[0] * B);
		s++;
	}
	return h; // or return h % C;
}

void Client::printScoreBoard() {
	sendServerCommand(REQUEST_SCOREBOARD, NULL);
}

bool Client::isNameTaken(char* userName) {
	this->userName = userName;
	handler->onCommandRecieved(0);
	if (sendServerCommand(CHECK_USERNAME, userName))
		return true;
	return false;
}

bool Client::registerUser(char* userName, unsigned enPass) {
	char buff[100];
	sprintf(buff, "%s::%u", userName, enPass);
	if (sendServerCommand(REGISTER, buff))
		return true;
	return false;
}

bool Client::LogInUser(char* userName, unsigned enPass) {
	char buff[100];
	sprintf(buff, "%s::%u", userName, enPass);
	if (sendServerCommand(LOGIN, buff))
		return true;
	return false;
}

void Client::getOnlinePlayers(char* userName) {
	sendServerCommand(SHOW_USER_LIST, userName);
}

void Client::run() {
	while (isConnected) {
		if (!isPlaying) {
			recvServerCommand();
		} else { //turns to true in the start game
			if (handler->startGame) {
				startTheGame();
			}
			handler->startGame = false;
			if (myTurn) {
				board();
				makeMove();
				myTurn = false;
			}
			recvClientCommand();
		}
	}
}

void Client::makeMove() {
	string lineNum;
	char* chosenNum;
	bool flag = false;
	do {
		if (flag)
			cout << "Invalid move, Please try again..." << endl;
		cin >> lineNum;
		chosenNum = (char*) (lineNum.c_str());
		flag = true;
	} while (!updateTable(chosenNum, mark)); // move was updated
	if (checkWin() != 1) {
		udpSocket->sendTo(chosenNum, "127.0.0.1", this->getPeerPort());
	} else {
		udpSocket->sendTo(chosenNum, "127.0.0.1", this->getPeerPort());
		cout << endl;
		cout << "Congrats, You're the winner!" << endl;
		isPlaying = false;
		handler->inGame = false;
		sendServerCommand(WON, NULL);
		sleep(1);
		printScoreBoard();
	}
}

void Client::startTheGame() {
	cout << "xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxo" << endl;
	cout << "x                                    o" << endl;
	cout << "x            Tic Tac Toe             o" << endl;
	cout << "x                                    o" << endl;
	cout << "xoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxoxo" << endl;
	cout << endl;
}

//prints the board to the screen
void Client::board() {
	cout << "     |     |     " << endl;
	cout << "  " << square[1] << "  |  " << square[2] << "  |  " << square[3]
			<< endl;
	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;

	cout << "  " << square[4] << "  |  " << square[5] << "  |  " << square[6]
			<< endl;

	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;

	cout << "  " << square[7] << "  |  " << square[8] << "  |  " << square[9]
			<< endl;

	cout << "     |     |     " << endl << endl;
	cout << "Your mark is " << mark << ", please enter a number." << endl;
}

void Client::sendRequest(char* userName) {
	sendServerCommand(SEND_REQUEST, userName);
}

void Client::acceptRequest() {
	sendServerCommand(ACCEPT_REQUEST, wantsToPlayWithMe);
	myTurn = true;
}

void Client::rejectRequest() {
	sendServerCommand(REJECT_REQUEST, wantsToPlayWithMe);

}

int Client::checkWin() {
	if (square[1] == square[2] && square[2] == square[3])

		return 1;
	else if (square[4] == square[5] && square[5] == square[6])

		return 1;
	else if (square[7] == square[8] && square[8] == square[9])

		return 1;
	else if (square[1] == square[4] && square[4] == square[7])

		return 1;
	else if (square[2] == square[5] && square[5] == square[8])

		return 1;
	else if (square[3] == square[6] && square[6] == square[9])

		return 1;
	else if (square[1] == square[5] && square[5] == square[9])

		return 1;
	else if (square[3] == square[5] && square[5] == square[7])

		return 1;
	else if (square[1] != '1' && square[2] != '2' && square[3] != '3'
			&& square[4] != '4' && square[5] != '5' && square[6] != '6'
			&& square[7] != '7' && square[8] != '8' && square[9] != '9')

		return 0;
	else
		return -1;
}

bool Client::updateTable(char* num, char myMark) {
	if (square[1] != '1' && square[2] != '2' && square[3] != '3'
			&& square[4] != '4' && square[5] != '5' && square[6] != '6'
			&& square[7] != '7' && square[8] != '8' && square[9] != '9')
		return false;
	square[*num - 48] = myMark;
	return true;
}


}
