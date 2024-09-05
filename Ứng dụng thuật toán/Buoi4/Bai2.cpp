#include <bits/stdc++.h>

using namespace std;

void xuat(float *a,int n){
	for(int i=0;i<n;i++)
		cout<<setprecision(3)<<a[i]<<" ";
}

void Merge(float a[], int l, int m, int r){
	vector<float> x(a + l, a + m + 1);
	vector<float> y(a + m + 1, a + r + 1);
	int i=0, j=0;
	while(i<x.size() && j<y.size()){
		if(x[i] < y[j]){
			a[l] = x[i];
			i++;
			l++;
		} else {
			a[l] = y[j];
			j++;
			l++;
		}
	}
	while(i<x.size()){
		a[l] = x[i];
		i++;
		l++;
	}
	while(j<y.size()){
		a[l] = y[j];
		j++;
		l++;
	}
}

void MergeSort(float a[], int l, int r){
	if(l<r){
		int m = (l+r)/2;
		MergeSort(a,l,m);
		MergeSort(a,m+1,r);
		Merge(a,l,m,r);
	}
}

int main() {
	int n=15;
	float *a;
	srand(time(NULL));
	a = new float[n];
	for(int i=0;i<n;i++)
		a[i] = (float) (rand()%100) / 3;
	xuat(a,n);
	MergeSort(a,0,n-1);
	cout<<"\nSau khi sap xep: "<<endl;
	xuat(a,n);
	return 0;
}

