/*
 * User.h
 *
 *  Created on: Aug 11, 2017
 *      Author: user
 */

#ifndef SRC_USER_H_
#define SRC_USER_H_
#include "TCPSocket.h"
using namespace std;
namespace npl {

class User :public TCPSocket{
private:
	char* userName, hashedPass;
public:
	User();
	virtual ~User();
};

} /* namespace npl */

#endif /* SRC_USER_H_ */
