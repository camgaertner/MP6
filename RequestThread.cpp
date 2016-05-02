#include "RequestThread.h"
#include <iostream>

RequestThread::RequestThread(string n, int r) {
	name = n;
	numRequests = r;
}

void RequestThread::run(BoundedBuffer& bb) {
	
	for(; numRequests > 0; numRequests--) {
		bb.push(name);
	}
}