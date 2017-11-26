/*
 * Socket.cpp
 *
 *  Created on: Jun 3, 2017
 *      Author: user
 */

#include "Socket.h"

namespace npl{

Socket::Socket(int type) {
	fsize = 0;
	socket_fd = socket(AF_INET, type, 0);
}

int Socket::bind(int port) {
	bzero((char *) &s_in, sizeof(s_in));

	s_in.sin_family = (short) AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY); /* WILDCARD any system ip*/
	s_in.sin_port = htons(port); //network byte order

	fsize = sizeof(from);

	if (::bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in)) < 0) {
		cout << "Error naming channel" << endl;
		return -1;
	}
	return 1;
}

int Socket::connect(const string& ip, int port) {
	bzero(&clientAddr, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr(ip.data());
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(port);
	if (::connect(socket_fd, (struct sockaddr *) &clientAddr,
			sizeof(clientAddr)) < 0) {
		perror("Error establishing communications");
		::close(socket_fd);
		return -1;
	}
	return 1;
}

void Socket::closeSock() {
	shutdown(socket_fd, SHUT_RDWR);
	::close(socket_fd);
	cout << "Socket has been closed!";
}

int Socket::read(char* buff, int length) {
	int read = ::read(socket_fd, buff, length);
	//cout << "socket" << buff << endl;
	return read;
}

int Socket::write(const string& msg) {
	return ::write(socket_fd, msg.data(), msg.size());
}
int Socket::send(const char* buffer, int len) {
	return ::write(socket_fd, buffer, len);
}

Socket::~Socket() {
}

string Socket::getLocalIp() {
	struct sockaddr_in temp;
	unsigned int size = sizeof(temp);
	bzero(&temp, sizeof(temp));
	getsockname(socket_fd, (struct sockaddr *) &temp, &size);
	return inet_ntoa(temp.sin_addr);
}

int Socket::getLocalPort() {
	return (ntohs(this->clientAddr.sin_port));
	//socklen_t addressLength = sizeof(clientAddr);
	//getsockname(socket_fd, (struct sockaddr*) &clientAddr, &addressLength);
	//return (int) ntohs(clientAddr.sin_port);
}
string Socket::getLocalAddr() {
	string peerIp = getLocalIp();
	int peerPort = getLocalPort();
	std::stringstream buff;
	buff << peerIp << "::" << peerPort;
	string peerAddress = buff.str();
	//		cout<<"Peer address is : "<< peerAddress<<endl;
	return peerAddress;
}

} /* namespace npl */
