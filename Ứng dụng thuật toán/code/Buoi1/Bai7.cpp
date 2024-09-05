#include <iostream>
using namespace std;

int fib[100];
bool isCal[100];

void init() {
	for(int i = 0 ; i < 100; ++i) {
		isCal[i] = false;
	}
	fib[1] = 1;
	fib[2] = 1;
	isCal[1] = true;
	isCal[2] = true; 
}

int fibo(int n) {
	if(isCal[n] == false) {
		fib[n] = fibo(n-1) + fibo(n-2);
		isCal[n] = true;
	}
	return fib[n] ;
}


int main() {
	init();
	cout<<fibo(10);
}
