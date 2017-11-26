/*
 * MThread.h
 *
 *  Created on: Feb 13, 2013
 *      Author: Omri
 */

#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <pthread.h>


class MThread{
public:
	pthread_t threadId;
	virtual void run() = 0;
	void start();
	void waitForThread();
	virtual ~MThread();
};


#endif /* MTHREAD_H_ */
