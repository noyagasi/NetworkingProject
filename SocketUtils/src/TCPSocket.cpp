/*
 * TCPSocket.cpp
 *
 *  Created on: May 4, 2017
 *      Author: Omri Grossman
 */

#include "TCPSocket.h"

namespace npl {
TCPSocket::TCPSocket(int port) :
		Socket(SOCK_STREAM) {
	int optval = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	bind(port);
}
TCPSocket::TCPSocket(string perrIP, int port) :
	Socket(SOCK_STREAM)
	{

		//struct sockaddr_in cli_name;
		//printf("Client is alive and establishing socket connection.\n");
		socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0) {
			perror("Error opening channel");
			::close(socket_fd);
		}
		bzero(&clientAddr, sizeof(clientAddr));
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		clientAddr.sin_port = htons(port);
		if (::connect(socket_fd, (struct sockaddr *) &clientAddr,
				sizeof(clientAddr)) < 0) {
			perror("Error establishing communications");
			::close(socket_fd);
		}
	}

	TCPSocket::TCPSocket(int connected_sock, struct sockaddr_in serverAddr,
			struct sockaddr_in peerAddr) :
	Socket(SOCK_STREAM) {
		socket_fd = connected_sock;
		this->serverAddr = serverAddr;
		clientAddr = peerAddr;
	}

	int TCPSocket::recv(char* buff, int len) {
		int res = read(buff, len);
		//cout << "SocketUtils::TCPSocket::recv" << endl;
		return res;
	}

	int TCPSocket::send(const char* msg, int len) {
		int res = Socket::send(msg, len);
		//cout << "SocketUtils::TCPSocket::send" << endl;
		return res;
	}

	string
	TCPSocket::fromAddr()
	{
		return inet_ntoa(clientAddr.sin_addr);
	}

	int TCPSocket::socket() {
		return socket_fd;
	}

	string
	TCPSocket::peerAddrToString()
	{
		char addr[30];
		sprintf(addr, "%d.%d.%d.%d:%d",
				int(this->clientAddr.sin_addr.s_addr & 0xFF),
				int((this->clientAddr.sin_addr.s_addr & 0xFF00) >> 8),
				int((this->clientAddr.sin_addr.s_addr & 0xFF0000) >> 16),
				int((this->clientAddr.sin_addr.s_addr & 0xFF000000) >> 24),
				ntohs(this->clientAddr.sin_port));
		return addr;
	}

	string
	TCPSocket::localAddrToString()
	{
		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		if (getsockname(this->socket_fd, (struct sockaddr *) &sin, &len)
				== -1) {
			perror("getsockname");
			return NULL;
		}
		char addr[30];
		sprintf(addr, "%d.%d.%d.%d:%d", int(sin.sin_addr.s_addr & 0xFF),
				int((sin.sin_addr.s_addr & 0xFF00) >> 8),
				int((sin.sin_addr.s_addr & 0xFF0000) >> 16),
				int((sin.sin_addr.s_addr & 0xFF000000) >> 24),
				ntohs(sin.sin_port));

		return addr;
	}

	int TCPSocket::getPort() {
		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		if (getsockname(this->socket_fd, (struct sockaddr *) &sin, &len) == -1) {
			perror("getsockname");
			return -1;
		}
		return ntohs(sin.sin_port);
	}

	TCPSocket * TCPSocket::listenAndAccept()
	{
		int rc = listen(socket_fd, 1);
		if (rc < 0) {
			return NULL;
		}
		socklen_t len = sizeof(clientAddr);
		bzero((char *) &clientAddr, sizeof(clientAddr));

		int connect_sock = accept(socket_fd, (struct sockaddr *) &clientAddr,
				&len);
		return new TCPSocket(connect_sock, serverAddr, clientAddr);
	}
}
