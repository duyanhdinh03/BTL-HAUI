#include<iostream>

using namespace std;

void show(int a[], int n){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

void Try(int k, int a[], int n){
	for(int i=0; i<=1; i++){
		a[k]=i;
		if(k==n){
			show(a,n);
		} else {
			Try(k+1,a,n);
		}
	}
}

int main(){
	int n, *a;
	cout<<"Nhap n: ";
	cin>>n;
	a = new int[n];
	Try(1,a,n);
}
