#include <iostream>
#include <stack>
using namespace std;
struct Call {
	char start;
	char end;
	char temp;
	int n;
};


void hanoiTower(int n) {
	stack<Call> calls;
	calls.push({'A','C','B',n});
	while(!calls.empty()) {
		Call now = calls.top();
		calls.pop();
		if(now.n == 1) {
			cout<<now.start<<"-->"<< now.end<<"\n";
		}
		else {
			calls.push({now.temp, now.end, now.start, now.n - 1});
			calls.push({now.start, now.end, now.temp, 1});
			calls.push({now.start, now.temp, now.end, now.n - 1});
		}
	}
}


int main() {
	int n;
	cin>>n;
	hanoiTower(n);
}
