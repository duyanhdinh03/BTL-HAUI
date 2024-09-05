#include<iostream>

using namespace std;

// Cach 1
void show(int a[], int n){
	for(int i=1;i<=n;i++)
		cout<<a[i];
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
	int n;
	cin>>n;
	sinh_hoan_vi(n);
}

//Cach 2
/*
int *dd;
int *a;
int n;

void show(int a[]){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

void Try(int k){
	for (int i=1;i<=n;i++){
		if (dd[i] == 0){
			a[k] = i;
			if (k == n){
				show(a);
			} else {
				dd[i] = 1;
				Try(k+1);
				dd[i] = 0;
			}
		}
	}
}

int main(){
	cin>>n;
	a = new int[n+1];
	dd = new int[n+1];
	for(int i=0;i<=n;i++)
		dd[i]=0;
	Try(1);
}
*/
