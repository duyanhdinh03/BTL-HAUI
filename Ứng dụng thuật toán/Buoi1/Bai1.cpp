#include<iostream>

using namespace std;

// Cach 1
void output(int a[], int n){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

void sinh_nhi_phan(int n){
	int i;
	int a[n+1] = {0};
	do{
		output(a,n);
		i=n;
		while(i>0 && a[i]==1){
			i--;
		}
		if(i>0){
			a[i]=1;
			i++;
			while(i<=n){
				a[i]=0;
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

//Cach 2
/*
int *a;
int n;

void show(int a[]){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

void Try(int k){
	for (int i=0;i<=1;i++){
		a[k] = i;
		if(k == n){
			show(a);
		} else {
			Try(k+1);
		}
	}
}

int main(){
	cin>>n;
	a = new int[n+1];
	for(int i=0;i<=n;i++)
		a[i] = 0;
	Try(1);
}
*/
