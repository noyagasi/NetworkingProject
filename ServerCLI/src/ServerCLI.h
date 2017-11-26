/*
 * ServerCLI.h
 *
 *  Created on: Aug 27, 2017
 *      Author: user
 */

#ifndef SERVERCLI_H_
#define SERVERCLI_H_
#include <iostream>
#include <string>
#include "ServerManager.h"
using namespace std;
using namespace npl;
class ServerCLI /*: public MThread*/ {
public:
	void runCLI();
	void printMenu();
	//void run();
};

#endif /* SERVERCLI_H_ */
