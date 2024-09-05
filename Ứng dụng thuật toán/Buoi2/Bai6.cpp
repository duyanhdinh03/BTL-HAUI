#include<iostream>

using namespace std;

void show(int a[], int n){
	for(int i=1; i<=n; i++)
		cout<<a[i];
	cout<<endl;
}

void Try(int k, int a[], int n, int dd[]){
	for(int i=1; i<=n; i++){
		if(dd[i] == 0){
			a[k] = i;
			if (k == n) {
				show(a,n);
			} else {
				dd[i] = 1;
				Try(k+1,a,n,dd);
				dd[i] = 0;
			}
		}
	}
}

int main(){
	int n, *a, *dd;
	cout<<"Nhap n: ";
	cin>>n;
	a = new int[n+1];
	dd = new int[n+1];
	for(int i=0; i<=n; i++)
		dd[i] = 0;
	Try(1,a,n,dd);
}
