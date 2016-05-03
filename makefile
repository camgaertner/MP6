# makefile

all: dataserver client

NetworkRequestChannel.o: NetworkRequestChannel.h NetworkRequestChannel.cpp
	g++ -c -g NetworkRequestChannel.cpp -std=c++11

dataserver: dataserver.cpp NetworkRequestChannel.o 
	g++ -g -o dataserver dataserver.cpp NetworkRequestChannel.o -lpthread

client: simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp StatisticsThread.cpp NetworkRequestChannel.o
	g++ -std=c++11 -pthread -g -o client simpleclient.cpp RequestThread.cpp BoundedBuffer.cpp Semaphore.cpp WorkerThread.cpp StatisticsThread.cpp NetworkRequestChannel.o
