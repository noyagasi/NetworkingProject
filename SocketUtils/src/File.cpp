/*
 * File.cpp
 *
 *  Created on: Jun 3, 2017
 *      Author: user
 */

#include "File.h"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

namespace npl {

File::File(char* path) {
	fileName=path;

}

void File::closeSock() {
		fclose(fPointer);
		cout<<"File closed!"<<endl;
}

int File::lengthOfFile(string name) {
	//fPointer=fopen(fileName, "r");
	//string name="noy";
	string line;
	//getline(fPointer,name.size(),name.data());
	//getline(name, name.size(), fPointer);


//	std::getline(fPointer, name.data());
//	fPointer->_


	ifstream is;
	is.open (fileName, ios::binary );
	int len;
	if(is==NULL)
		cout<<"file is null"<<endl;
	while(std::getline(is,name)){
		if(line.find(name)!=string::npos){
			cout<<"the line found is: "<<line<<endl;
			return 1;
		}
	}
	cout<<"didn't find name"<<endl;
	return -1;

//	is.seekg (0, ios::end);
//	int length = is.tellg();
//	//cout<<length<<endl;
//	return 0;
}

int File::read(char* buff, int size) {
	fPointer=fopen(fileName, "r");
	/*while(!feof(fPointer)){
		fgets(buff,size+1,fPointer);
		puts(buff);
	}*/
	while(fgets(buff, size, fPointer) != NULL)
		puts(buff);

	//while((ch = fgetc(file_ptr))!= EOF) /* get a char from disk */printf("%c", ch);

	return 1;
}

int File::write(const string& msg) {
	fPointer=fopen(fileName, "w");
	fprintf(fPointer,msg.data());
	fflush(fPointer);
	return 1;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

} /* namespace npl */


