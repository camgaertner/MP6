/* 
    File: simpleclient.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 2013/01/31

    Simple client main program for MP3 in CSCE 313
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
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <cstdlib>
#include "NetworkRequestChannel.h"
#include "RequestThread.h"
#include "EventThread.h"
#include "StatisticsThread.h"
#include "BoundedBuffer.h"

#include <thread>
#include <future>         // std::async, std::future
#include <chrono> 
#include <functional>
#include <vector>
#include <mutex>

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
	
	for(int i = 0; i < 1; i++) {
		int pid = fork();
		if(pid == 0) {
			execvp("./client", NULL);
			return 0;
		}
	}
	auto begin = std::chrono::steady_clock::now();
	int data_requests = 0;
	int bounded_buffer_size = 0;
	int request_channels = 0;
	string hostname = "localhost";
	int port = 3000;
    int opt;
	//
    while ((opt = getopt(argc, argv, "n:b:w:h:p:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            data_requests = atoi(optarg);
            break;
        case 'b':
            bounded_buffer_size = atoi(optarg);
            break;
        case 'w':
        	request_channels = atoi(optarg);
        	break;
		case 'h': 
			hostname = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
        }
    }
    if (data_requests == 0)
        data_requests = 1000; //default is 128
   
    if(bounded_buffer_size == 0)
        bounded_buffer_size = 5;  //default is 512kb

    if(request_channels == 0)
    	request_channels = 5;

	/*int pid = fork();
	if(pid == 0) {
		execvp("./dataserver", NULL);
		return 0;
	}*/
  // fill ints from arguments later
	// int data_requests = 1000;
	// int bounded_buffer_size = 5;
	// int worker_threads = 5;
	cout << "CLIENT STARTED:" << endl;
	cout << "Establishing control channel... " << flush;
	NetworkRequestChannel chan (hostname, port);
	cout << "done." << endl;
	
  
	BoundedBuffer requests(bounded_buffer_size);
	
	BoundedBuffer response1(bounded_buffer_size);
	BoundedBuffer response2(bounded_buffer_size);
	BoundedBuffer response3(bounded_buffer_size);
	
	vector<BoundedBuffer> buffers = {response1, response2, response3};
	
	RequestThread r1 ("Joe Smith", data_requests);
	RequestThread r2 ("Jane Smith", data_requests);
	RequestThread r3 ("John Doe", data_requests);

	StatisticsThread s1 ("Joe Smith");
	StatisticsThread s2 ("Jane Smith");
	StatisticsThread s3 ("John Doe");
	
	vector<thread> threads;
	
	threads.push_back(thread([&]() { r1.run(requests); }));
	threads.push_back(thread([&]() { r2.run(requests); }));
	threads.push_back(thread([&]() { r3.run(requests); }));
	
	vector<NetworkRequestChannel*> reqs;
	for(int i = 0; i < request_channels; i++) {
		reqs.push_back(new NetworkRequestChannel(hostname, port));
	}
	EventThread eventThread = EventThread();
	threads.push_back(thread([&]() { eventThread.run(requests, reqs, buffers, data_requests); }));
	
	threads.push_back(thread([&]() { s1.run(buffers[0], data_requests); }));
	threads.push_back(thread([&]() { s2.run(buffers[1], data_requests); }));
	threads.push_back(thread([&]() { s3.run(buffers[2], data_requests); }));
	
	for(int i = 0; i < threads.size() - 3; i++) {
		threads[i].join();
	}
	for(int i = 0; i < reqs.size(); ++i) {
		reqs[i]->send_request("quit");
	}
	string reply4 = chan.send_request("quit");
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count() <<std::endl;
}
