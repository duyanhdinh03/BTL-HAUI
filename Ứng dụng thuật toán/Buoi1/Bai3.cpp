#include<iostream>

using namespace std;

void show(int a[], int k){
	for(int i=1;i<=k;i++)
		cout<<a[i];
	cout<<endl;
}

void next(int a[], int k, int i){
	a[i] += 1;
	i++;
	while(i<=k){
		a[i] = a[i-1] + 1;
		i++;
	}
}

void sinh_tap_con(int n, int k){
	int i, a[k+1] = {0};
	for(int i=1;i<=k;i++)
		a[i] = i;
	do{
		show(a,k);
		i=k;
		while(i>0 && a[i]==n-k+i){
			i--;
		}
		if(i>0){
			next(a,k,i);
		}
	}while(i>0);
}

int main(){
	int n,k;
	cout<<"Nhap n: ";
	cin>>n;
	cout<<"Nhap k: ";
	cin>>k;
	
	sinh_tap_con(n,k);
}
