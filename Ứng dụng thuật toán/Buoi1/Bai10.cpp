#include<iostream>

using namespace std;

long tongle(int *a, int n){
	if(n==0)
		if(a[n]%2==0)
			return 0;
		else
			return a[n];
	if(a[n]&2!=0)
		return a[n] + tongle(a,n-1);
	return tongle(a,n-1);
}

int main(){
	int n;
	cin>>n;
	int *a = new int[n];
	for(int i=0;i<n;i++)
		cin>>a[i];
	cout<<tongle(a,n);
}
