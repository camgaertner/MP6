#include "WorkerThread.h"
#include <iostream>
#include <future>

using namespace std;

void WorkerThread::run(BoundedBuffer& bb, mutex& lock, vector<BoundedBuffer>& responseBuffers, string hostname, int port) {

	lock.lock();
	NetworkRequestChannel mychan(hostname, port);
	lock.unlock();
	while(true) {
	
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(1);

		auto promise = std::promise<string>();
		auto f = promise.get_future();

		thread([&]
		{
			promise.set_value(bb.pop());
		}).detach();

		/*std::future<string> fut = std::async (std::launch::async, [&](){ return bb.pop(); });
		
		string answer = "";
		thread t ([&](answer&) { answer = bb.pop(); }).detach();
	*/	
		if(std::future_status::ready == f.wait_until(one)) { 
			//std::cout << "Completed!" << endl;
		}
		else
		{
			mychan.send_request("quit");
			promise.set_value("asdfhasdkf");
			f.get();
			break;
		}
		string data = f.get();

		string reply = mychan.send_request("data " + data);
		

		if(data == "Joe Smith") {
			responseBuffers[0].push(reply);
		}
		else if(data == "Jane Smith") {
			responseBuffers[1].push(reply);
		} 
		else if(data == "John Doe"){
			responseBuffers[2].push(reply);
		}
	}
}
