#include "BoundedBuffer.h"
#include <string>

class RequestThread {
	
public:
	RequestThread(string, int);
	
	void run(BoundedBuffer&);
	
private:
	string name;
	int numRequests;

};