#include "NetworkRequestChannel.h"
#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>

#define MAXDATASIZE 500 

using namespace std;

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no) {
	int numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	
	
	string port = std::to_string(_port_no);
	
    if ((rv = getaddrinfo(_server_host_name.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);
	
}

NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (int *)) { 
	struct sockaddr_in my_addr;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	int option = 1;
	setsockopt(sockfd,SOL_SOCKET,(SO_BROADCAST | SO_REUSEADDR),(char*)&option,sizeof(option));
	printf("Socket is %d\n", sockfd);
	if(sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(_port_no);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
	
	listen(sockfd, 5);
	
	if (listen(sockfd, 10) == -1) {
        perror("listen");
        exit(1);
    }

	struct sockaddr_in their_addr;
	socklen_t addr_size;
	while(1)
	{
		std::cout << "About to find a connection" << std::endl;
		int connection = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
		if(connection >= 0)
		{	
			std::cout << "Found connection" << connection << "! Setting up..." << std::endl;
			pthread_t thread;
			pthread_create(&thread, NULL, connection_handler, (int*)connection);
			pthread_detach(thread);
		}
	}
}

NetworkRequestChannel::~NetworkRequestChannel() {
	close(sockfd);
}

string NetworkRequestChannel::send_request(string _request) {
	cwrite(_request);
	string s = cread(); 
	return s;
}

const int MAX_MESSAGE = 255;

string NetworkRequestChannel::cread() {
	char buf[MAX_MESSAGE];

	if (read(sockfd, buf, MAX_MESSAGE) < 0) {
		perror(string("Error reading from socket").c_str());
	}

	string s = buf;

	return s;
}

int NetworkRequestChannel::cwrite(string _msg) {
	if (_msg.length() >= MAX_MESSAGE) {
		cerr << "Message too long for Channel!\n";
		return -1;
  }
  const char * s = _msg.c_str();

  if (write(sockfd, s, strlen(s)+1) < 0) {
    perror(string("Error writing to socket").c_str());
  }
  
}

void asdf(int* a) {
	cout << "connection handleer";
}

void connection_handler(int fd) {
	
}

int main() {
	NetworkRequestChannel net(1234, asdf);
	
}