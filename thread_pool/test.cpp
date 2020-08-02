#include <iostream>
#include "locker.h"
#include "threadpool.h"

using namespace std;

class node {
	public:
		int num;
		node() {
			num = 0;
		}
		node(int n) {
			num = n;
		}
		void process() {
			printf( "process %dth \n", num );
		}
};
int main () {
	threadpool<node> *pool = NULL;
	pool = new threadpool<node>();

	for (int i= 0; i< 100; i++) {
		pool->append(new node(i));
	}
	cout<< "append finished!"<< endl;
	cin.get();
	return 0;
}
