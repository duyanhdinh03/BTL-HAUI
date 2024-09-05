#include <iostream>
using namespace std;
bool res[100];
int n;


void init() {
	cin>>n;
	for(int i = 0 ; i < n ; ++i) 
		res[i] = 0;
}

void show() {
	for(int i = 0; i < n ; ++i) {
		cout<<res[i];
	}
	cout<<"\n";
}

void gen(int k=0) {
	res[k] = 0;
	if(k == n-1)
		show();
	else
		gen(k+1);
	res[k] = 1;
	if(k == n-1)
		show();
	else
		gen(k+1);
}

int main() {
	init();
	gen();
}
