#include<bits/stdc++.h>

using namespace std;

int n;
int *a;

void show(){
	for(int i=1;i<=n;i++)
		cout<<setw(6)<<a[i];
	cout<<endl;
}

bool check(int x, int y){
	for(int i=1;i<x;i++)
		if(a[i] == y || abs(i-x) == abs(a[i]-y))
			return false;
	return true;
}

void Try(int k){
	for(int i=1;i<=n;i++){
		if(check(k,i)){
			a[k] = i;
			if(k==n)
				show();
			Try(k+1);
		}
	}
}

int main(){
	cout<<"Nhap do dai ban co: ";
	cin>>n;
	a = new int [n+1];
	cout<<endl;
	for(int i=1;i<=n;i++)
		cout<<left<<"cot"<<i<<"  ";
	cout<<endl;
	Try(1);
}
