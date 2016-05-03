#include "EventThread.h"
#include <iostream>
#include <future>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <numeric>
#include <iomanip>

EventThread::EventThread() {
	
}

void EventThread::run(BoundedBuffer& bb, vector<NetworkRequestChannel*> reqs, vector<BoundedBuffer>& response_buffers, int data_requests) {
	int total = data_requests * 3;
	int sent = 0;
	int received = 0;
	struct timeval tv;
	tv.tv_sec = 0;
    tv.tv_usec = 50;
	for(int i = 0; i < reqs.size(); ++i) {
		reqs[i]->cwrite("data " + bb.pop());
		sent++;
	}
	int count = 0;
	while(true) {
		//cout << "Count: " << ++count << endl;
		count++;
		fd_set rdset;
		FD_ZERO(&rdset);
		int max = 0;
		for(int i = 0; i < reqs.size(); ++i) {
			int fd = reqs[i]->sockfd;
			FD_SET(fd, &rdset);
			if(fd > max)
				max = fd;
		}
		//cout << "select " << endl;
		int nr = select(max, &rdset, NULL, NULL, &tv);
		for(int i = 0; i < reqs.size(); ++i) {
			if(FD_ISSET(reqs[i]->sockfd, &rdset)) {
				string reply = reqs[i]->cread();
				if(received < data_requests) {
					response_buffers[0].push(reply);
				}
				else if(received >= data_requests && received < data_requests * 2) {
					response_buffers[1].push(reply);
				} 
				else{
					response_buffers[2].push(reply);
				}
				received++;
				cout << "Count is " << received <<  " Reply is " << reply << endl;
				
				if(sent < total) {
					reqs[i]->cwrite("data " + bb.pop());
					sent++;
					cout << "Sent is " << sent << endl;
				}
			}
		}
		
		if(received == total - 1){
			response_buffers[2].push("5");
			break;
		}
	}
	
	cout << "Exiting event thread" << endl << flush;
}
