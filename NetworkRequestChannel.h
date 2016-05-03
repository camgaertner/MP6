#ifndef NETWORK_REQUEST_CHANNEL
#define NETWORK_REQUEST_CHANNEL
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <iostream>
#include <string>

using namespace std;

class NetworkRequestChannel {

public:
	NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no);

	NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (int *));

	NetworkRequestChannel(int);

	~NetworkRequestChannel();

	string send_request(string _request);

	string cread();

	int cwrite(string _msg);
	
	int sockfd;
};


#endif
