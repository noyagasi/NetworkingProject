/*
 * TCPMessengerProtocol.h
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3347

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6

//Server
#define REGISTER					7
#define REGISTER_SUCCESSFUL			8
#define REGISTER_FAIL				9
#define CHECK_USERNAME				10
#define TAKEN_USERNAME				11
#define AVAILABLE_USERNAME			12


#define LOGIN						13
#define LOGIN_SUCCESSFUL			14
#define LOGIN_FAIL					15


#define SHOW_USER_LIST				16
#define OPEN_RANDOM_SESSION			17

#define SEND_REQUEST				18
#define ACCEPT_REQUEST				19
#define REJECT_REQUEST				20

#define START_GAME					21

#define FINISHED_GAME				22

#define WON							23
#define LOST						24

#define REQUEST_SCOREBOARD			25
#define PRINT_SCOREBOARD			26


#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"



#endif /* TCPMESSENGERPROTOCOL_H_ */
