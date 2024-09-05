#include<iostream>

using namespace std;

int uscln(int a, int b){
	if(a%b==0)	return b;
	return uscln(b,a%b);
}

int main(){
	int a, b;
	cout<<"Nhap a: ";
	cin>>a;
	cout<<"Nhap b: ";
	cin>>b;
	cout<<"Uoc so chung lon nhat: "<<uscln(a,b);
}
