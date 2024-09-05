#include<iostream>

using namespace std;

void show(int a[], int n){
	string name[6] = {"Trang","Cong","Trung","Binh","Hoan","Mai"};
	for(int i=1;i<=n;i++){
		cout<<name[a[i]-1]<<" ";
	}
	cout<<endl;
}

void next(int a[], int n, int i){
	int k=n;
	while(a[k]<a[i]){
		k--;
	}
	swap(a[k], a[i]);
	int j=n;
	i++;
	while(i<j){
		swap(a[i], a[j]);
		i++;
		j--;
	}
}

void sinh_hoan_vi(int n){
	int i, a[n+1]={0};
	for(i=1;i<=n;i++)
		a[i] = i;
	do{
		show(a,n);
		i=n-1;
		while(i>0 && a[i]>a[i+1]){
			i--;
		}
		if(i>0){
			next(a,n,i);
		}
	}while(i>0);
}

int main(){
	sinh_hoan_vi(6);
}
