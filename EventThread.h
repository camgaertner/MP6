#include "BoundedBuffer.h"
#include <sys/select.h>
#include "NetworkRequestChannel.h"
#include <string>

class EventThread {
	
public:
	EventThread();
	
	void run(BoundedBuffer&, vector<NetworkRequestChannel*>, vector<BoundedBuffer>&, int);

};
