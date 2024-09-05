#include<iostream>

using namespace std;

int dem=0;

void chuyen(int n, char a, char b, char c){
	if(n==1){
		cout<<dem+1<<": "<<a<<" sang "<<c<<endl;
		dem++;
	}	
	else{
		chuyen(n-1,a,c,b);
		chuyen(1,a,b,c);
		chuyen(n-1,b,a,c);
	}
}

int main(){
	int n;
	cin>>n;
	chuyen(n,'A','B','C');
	cout<<"So lan chuyen: "<<dem;
}
