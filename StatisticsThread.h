#include "BoundedBuffer.h"
#include <string>

class StatisticsThread {
	
public:
	StatisticsThread(string);
	
	void run(BoundedBuffer&, int);
	
private:
	string name;
};
