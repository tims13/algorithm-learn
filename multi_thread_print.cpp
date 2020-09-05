#include <iostream>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <thread>

using namespace std;

const int PRINT_NUM = 10;
mutex m_mutex;
condition_variable m_cv;

int cur = 0;

void mprint(int n) {
	for (int i= 0; i< PRINT_NUM; i++) {
		unique_lock<mutex> m_lock(m_mutex);
		m_cv.wait(m_lock, [n] {return n == cur;});
		cout << (char)('A' + n) << endl;
		cur = (cur+1)%3;
		m_lock.unlock();
		m_cv.notify_all();
	}
} 

int main() {
	vector<thread> threads;
	threads.push_back(thread(mprint, 0));
	threads.push_back(thread(mprint, 1));
	threads.push_back(thread(mprint, 2));
	
	for (auto &t: threads) {
		t.join();
	}
	system("pause");
}
