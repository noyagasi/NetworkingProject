/*
 * FileInterface.h
 *
 *  Created on: Jun 3, 2017
 *      Author: user
 */

#ifndef FILEINTERFACE_H_
#define FILEINTERFACE_H_

#include <iostream>
#include "string"
#include "string.h"


using namespace std;
namespace npl {

class FileInterface {
public:
	virtual void closeSock();
	virtual int read(char* buff, int size);
	virtual int write(const string& msg);

//	FileInterface();
//	virtual ~FileInterface(){};
};

} /* namespace npl */

#endif /* FILEINTERFACE_H_ */
