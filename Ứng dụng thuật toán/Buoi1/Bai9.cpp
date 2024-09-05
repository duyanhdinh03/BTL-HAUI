#include<iostream>

using namespace std;

long tong(int n){
	if(n==0)
		return 0;
	return n + tong(n-1);
}

int main(){
	int n,sum=0;
	cin>>n;
	cout<<tong(n);
}
