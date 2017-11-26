/*
 * UDPSocket.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: user
 */

#include "UDPSocket.h"


namespace npl {

UDPSocket::UDPSocket(int port):Socket(SOCK_DGRAM){
	bind(port);
}

/**
 * 	- reads an incoming message from the UDP socket. the message is
 * 	copied into the given buffer up to the specified length.
 */
int UDPSocket::recv(char* buffer, int length){
	int n= recvfrom (socket_fd,buffer,length,0,(struct sockaddr *)&from,&fsize);
	return n;
}

/**
 * 	- sends the given message as a UDP message
 * 	to the given address specified by IP and port.
 *
 */
/*int UDPSocket::sendTo(const string& msg,const string& ip, int port){
	struct sockaddr_in s_in;
	bzero ((char *) &s_in, sizeof (s_in));
	s_in.sin_family = ( short )AF_INET;
	s_in.sin_addr.s_addr =  inet_addr(ip.data());
	s_in.sin_port = htons ( port);
	int n= sendto (socket_fd,msg.data(), msg.length(),0,(struct sockaddr*)&s_in,sizeof(s_in));
	if (n < 0){
		cout<<"Sending Error!";
		return -1;
	}
	return n;
}*/
int UDPSocket::sendTo(string msg, string ip, int port){
	//TODO: create an address struct for the destination
	struct sockaddr_in s_in;
		bzero((char *) &s_in, sizeof(s_in));
		s_in.sin_family = AF_INET;
		s_in.sin_addr.s_addr = inet_addr(ip.data());
		s_in.sin_port = htons(port);
		//TODO: call send to and sent the message to the destination address
		return sendto(socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&s_in,sizeof(s_in));
	}





/**
 * 	- reply to an incoming message,
 * 	this method will send the given message as a UDP
 * 	message to the peer from which the last message was received.
 *
 */
int UDPSocket::reply(const string& msg){
	int n= sendto (socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&from,sizeof(from));
	return n;
}
/**
 * - close the UDP socket.
 */
//void UDPSocket::closeSocket(){
//
//}

/**
 * - return the sender IP of the last received message.
 */
string UDPSocket::fromAddr(){
	return inet_ntoa(from.sin_addr);
}
}

