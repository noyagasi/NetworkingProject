/*
 * Socket.h
 *
 *  Created on: Jun 3, 2017
 *      Author: user
 */

#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_
#include <iostream>
#include <strings.h>
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <string.h>
#include <cstdio>
#include <netdb.h>
#include <stdio.h>
#include <sstream>

#include "FileInterface.h"

using namespace std;

namespace npl {

class Socket: public FileInterface {
protected:
	struct sockaddr_in s_in;
	struct sockaddr_in from;
	struct sockaddr_in serverAddr, clientAddr;
	int socket_fd;
	unsigned int fsize;
public:
	virtual int bind(int port);
	virtual int connect(const string& ip, int port);
	virtual int getLocalPort();
	virtual string getLocalIp();
	virtual string getLocalAddr();
	virtual int getSockFD() {
		return socket_fd;
	}
	//FileInterface part:
	virtual void closeSock();
	virtual int read(char* buff, int length);
	virtual int write(const string& msg);
	virtual int send(const char* buffer, int len);

	Socket(int type);
	virtual ~Socket();
};

} /* namespace npl */

#endif /* SRC_SOCKET_H_ */
