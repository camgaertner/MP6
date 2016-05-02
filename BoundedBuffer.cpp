/* 
    BoundedBuffer.cpp
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include "Semaphore.h"
#include "BoundedBuffer.h"

using namespace std;

BoundedBuffer::BoundedBuffer(int maxSize)
	: mutex(1), fullSlots(0), emptySlots(maxSize) {
	this->maxSize = maxSize;
}
void BoundedBuffer::push(string s) {
	emptySlots.P();
	mutex.P();
	buffer.push_back(s);
	mutex.V();
	fullSlots.V();
}
string BoundedBuffer::pop() {
	fullSlots.P();
	mutex.P();
	string val = buffer.at(0);
	buffer.erase(buffer.begin());
	mutex.V();
	emptySlots.V();
	return val;
}