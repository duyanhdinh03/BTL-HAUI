#include <iostream>
using namespace std;

int fibo(int n) {
	int prev = 1, next = 1;
	n--;
	while(n--) {
		int temp = prev;
		prev = next;
		next = next + temp;
	}
	return prev;
}


int main() {
	int n ; cin>>n;
	cout<<fibo(n);
}
