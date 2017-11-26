/*
 * Client.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#define A 54059 // a prime /
#define B 76963 // another prime /
#define C 86969 // yet another prime /
#define FIRSTH 37 // also prime /

#include "Guard.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include "TCPSocket.h"
#include "MThread.h"
#include "UDPSocket.h"
#include "MTCPListener.h"
#include "TCPMessengerProtocol.h"
#include "Socket.h"

using namespace std;
namespace npl {
class ClientHandler {
public:
	bool startGame;
	bool inGame;
	//handles with the recieved command
	virtual void onCommandRecieved(int cmd)=0;
	//handles the message we got
	virtual void onMessageRecieved(string message)=0;
	//the session is open, gets the ip and port of the other side
	virtual void onOpenSession(string ip, int port)=0;
	//the session is close
	virtual void onClosedSession()=0;
	//other player requests to connect me
	virtual void onOtherPlayerReq(char* username)=0;

};

class Client: public MThread {
private:
	pthread_mutex_t mutex;
	int peerPort;
	ClientHandler *handler;
	TCPSocket* tcpSocket;
	UDPSocket* udpSocket;
	bool isConnected;
	bool myTurn;
	string square;
	MTCPListener listener;
	char* userName, pass;
	unsigned hashedPass;
	char* wantsToPlayWithMe;
	char mark;
	bool wasMarked;
	bool isPlaying;
	int sendServerCommand(int cmd, const char* buff);
	int recvServerCommand();
	void board();
	void recvClientCommand();
	void makeMove();
	int checkWin();
public:
	bool updateTable(char* num,char peerMark);

/*	int sendStupidMsg(string msg, string ip, int port){
		udpSocket->sendTo(msg, "127.0.0.1", this->getPeerPort());
	}*/


	Client(ClientHandler* handler);
	virtual ~Client();
	/**
	 * connects the client to the remote server on the given ip,
	 * the server port is read from the protocol file
	 *
	 * @param ip - the ip of the server to connect to
	 *
	 * @return true on success otherwise false
	 */
	bool connect(string ip);

	/**
	 * opens the session to the other client via the server
	 *
	 * @param ip - the ip of the client we open the session with
	 * @param port - the port of the client we open the session with
	 *
	 * @return true on success otherwise false
	 */
	//bool openSession(string ip, int port);

	/**
	 * closes the session to the other client via the server
	 */
	//void closeSession();

	/**
	 * disconnect from the server
	 */
	void disconnect();

	/**
	 * exit the messenger
	 */

	void exit();
	/**
	 * returns the port from the remote server
	 */
	int getPort();

	unsigned hashPass(const char* s);

	bool isNameTaken(char* userName);

	bool registerUser(char* userName, unsigned enPass);

	bool LogInUser(char* userName, unsigned enPass);
	/**
	 * asks for a list of online players but userName
	 *
	 * @param userName - it's me, the one i don't want in the list
	 */
	void getOnlinePlayers(char* userName);
	/**
	 * request to start a game with userName
	 *
	 * @param userName - the player to start the game with
	 */
	void sendRequest(char* userName);

	/**
	 * accepts the request from the client that was trying to connect
	 */
	void acceptRequest();
	/**
	 * rejects the request from the client that was trying to connect
	 */
	void rejectRequest();

	/*
	 * starts the game with the remote client
	 */
	void startTheGame();

	/*
	 * ends the game and with result
	 */
	char stopPlaying();

	void run();

	void printScoreBoard();

	int getPeerPort() const {
		return peerPort;
	}


};

}

#endif /* CLIENT_H_ */

