#include <iostream>

using namespace std;

int uscln(int a, int b){
	int r = a % b;
	while(r!=0){
		a = b;
		b = r;
		r = a%b;
	}
	return b;
}

int main(){
	int a, b;
	cout<<"Nhap a: ";
	cin>>a;
	cout<<"Nhap b: ";
	cin>>b;
	cout<<"Uoc so chung lon nhat: "<<uscln(a,b);
}


