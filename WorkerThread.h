#include "BoundedBuffer.h"
#include "NetworkRequestChannel.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <mutex>
//

class WorkerThread {
	
public:
	
	void run(BoundedBuffer&, mutex&, vector<BoundedBuffer>&, string, int);
	
private:
	string name;
	int numRequests;

};
