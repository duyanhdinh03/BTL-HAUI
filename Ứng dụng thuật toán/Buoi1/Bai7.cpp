#include<bits/stdc++.h>

using namespace std;

int a[2000];

// O(n)
long f1(int n){
	if (n == 0 || n==1)
		return n;
	if(a[n] != 0)
		return a[n];
	else
		a[n] = f1(n-1) + f1(n-2);
	return a[n];
}

// O(1)
long f2(int n){
	if (n<=1) return n;
	long first = 0, second = 1,a,b;
	for(int i=2;i<=n;i++){
		a = first;
		b = second;
		first = b;
		second = a + b;
		
	}
	return second;
}

int main(){
	int n;
	cin>>n;
	for (int i=1;i<=n;i++)
		cout<<f1(i)<<" ";
	cout<<endl;
	for (int i=1;i<=n;i++)
		cout<<f2(i)<<" ";
}
