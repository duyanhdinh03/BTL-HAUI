#include<iostream>

using namespace std;

void output(char a[], int n){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

void sinh_nhi_phan(int n){
	int i;
	char *a = new char[n+1];
	for(int i=1;i<=n;i++)
		a[i] = 'a';
	do{
		output(a,n);
		i=n;
		while(i>0 && a[i] == 'b'){
			i--;
		}
		if(i>0){
			a[i] = 'b';
			i++;
			while(i<=n){
				a[i] = 'a';
				i++;
			}
		}
	}while(i>0);
}

int main(){
	int n;
	cout<<"Nhap do dai chuoi nhi phan: ";
	cin>>n;
	sinh_nhi_phan(n);
}
