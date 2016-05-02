#include "StatisticsThread.h"
#include <iostream>
#include <future>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <numeric>
#include <iomanip>

StatisticsThread::StatisticsThread(string n) {
	name = n;
}

void StatisticsThread::run(BoundedBuffer& bb, int total) {

	std::unordered_map<string, int> counters;
	cout << "STAT THREAD STARTED " << endl;
	int counter = 0;
	
	while(true) {
		std::chrono::system_clock::time_point one = std::chrono::system_clock::now() + std::chrono::seconds(2);

		string data = bb.pop();
		cout << endl << ++counter << endl << flush;
		counters[data]++;

		if(counter == total) 
		{
			string filename = name + ".txt";
			ofstream ost{filename, ofstream::out};

			vector<string> key_list;
			for (auto it=counters.begin(); it != counters.end(); ++it) {
		    	key_list.push_back(it->first);
			}
			sort(key_list.begin(), key_list.end(), [](const string& a, const string& b) -> bool 
				{
					return stoi(a) < stoi(b);
				});

			for(auto kv : key_list) {
				ost << name << ": " << setw(4) << kv << " appeared " << setw(6) << counters[kv] << " time(s)" << endl;
			}
			ost.close();
		}

		// auto promise = std::promise<string>();
		// auto f = promise.get_future();

		// thread([&]
		// {
		// 	promise.set_value(bb.pop());
		// 	cout << "POPPED!" << endl;
		// }).detach();

		// if(std::future_status::ready == f.wait_until(one)) { 
		// 	//std::cout << "Completed!" << endl;
		// }
		// else
		// {
		// 	promise.set_value("");
		// 	f.get();
		// 	cout << "STAT EXITING" << endl;
		// 	break;
		// }

		// string data = f.get();
	}
	cout << "EXITED STAT THREAD" << endl;
	string filename = name + ".txt";
	ofstream ost{filename, ofstream::out};

	vector<string> key_list;
	for (auto it=counters.begin(); it != counters.end(); ++it) {
    	key_list.push_back(it->first);
	}
	sort(key_list.begin(), key_list.end());
	int sum = 0;
	for(auto kv : key_list) {
		sum = sum + counters[kv];
		ost << name << ": " << kv << " appeared " << counters[kv] << " time(s)" << endl << "total: " << sum << endl;
	}
	ost.close();
}
