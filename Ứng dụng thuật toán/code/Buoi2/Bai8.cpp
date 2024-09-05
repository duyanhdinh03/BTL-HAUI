#include <iostream>
using namespace std;
bool mark[100]; 
int n; 
int res[100]; 

void init() {
	cin>>n; 
	for(int i = 0 ; i < n ; ++i) {
		res[i] = i; 
		mark[i] = 0;  
	}
}


void show() {
	for(int i = 0 ; i < n ; ++i) {
		cout<<res[i]+1<<" ";
	}
	cout<<"\n";
}

void gen(int k = 0) {
	
	for(int i = 0; i < n; ++i) {
		if(mark[i]==false) {
			mark[i] = true;
			res[k] = i;
			if(k == n - 1)
				show();
			else {
				gen(k+1);
			}
			mark[i] = false;
		}
	}
}
 

int main() {
	init();
	gen();
}
