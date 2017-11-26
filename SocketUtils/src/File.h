/*
 * File.h
 *
 *  Created on: Jun 3, 2017
 *      Author: user
 */

#ifndef SRC_FILE_H_
#define SRC_FILE_H_
#include <iostream>
#include <string.h>
#include <fstream>

#include "FileInterface.h"
using namespace std;

namespace npl {

class File :public FileInterface{
	FILE* fPointer;
	char* fileName;
	char data[1024];
public:
	File(char* path);
	void closeSock();
	//Getting the length of a file in characters
	int lengthOfFile(string name);
	int read(char* buff, int size);
	int write(const string& msg);
	virtual ~File();
};

} /* namespace npl */

#endif /* SRC_FILE_H_ */
