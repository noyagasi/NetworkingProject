//============================================================================
// Name        : OverAllTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MainTester.h"
using namespace std;

int main() {
	MainTester tester = MainTester();
	if(tester.test())
		cout<<"Test Passed!"<<endl;
	else cout<<"Test Failed!"<<endl;
	return 0;
}
