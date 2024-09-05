#include <bits/stdc++.h>

using namespace std;

float AmuN(float a, int n){
	if(n==0)
		return 1;
	if(n==1)
		return a;
	float t = AmuN(a, n/2);
	if(n%2==0)
		return t*t;
	else
		return t*t*a;
}

int main() {
	float a;
	int n;
	cout<<"Nhap a: ";
	cin>>a;
	cout<<"Nhap n: ";
	cin>>n;
	cout<<"a mu n: "<<AmuN(a,n);
	return 0;
}

