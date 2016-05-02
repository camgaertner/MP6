/* 
    File: BoundedBuffer.h


*/

#ifndef BOUNDED_BUFFER_H                   // include file only once
#define BOUNDED_BUFFER_H

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
#include <vector>
#include "Semaphore.h"
#include <string>
using namespace std;
class BoundedBuffer {
public:
	int maxSize;
	vector<string> buffer;
	Semaphore mutex;
	Semaphore emptySlots;
	Semaphore fullSlots;
	BoundedBuffer(int maxSize);
	void push(string s);
	string pop();
};


#endif


