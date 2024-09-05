#include<iostream>

using namespace std;

int fibo(int n){
	if(n<=1)
		return n;
	int a = 0,b = 1;
	for(int i=2;i<=n;i++){
		int c = a + b;
		a = b;
		b = c;
	}
	return b;
}

int main(){
	int n;
	cin>>n;
	for(int i=1;i<=n;i++)
		cout<<fibo(i)<<" ";
}
