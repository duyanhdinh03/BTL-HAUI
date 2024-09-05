#include <bits/stdc++.h>

using namespace std;

void xuat(float *a,int n){
	for(int i=0;i<n;i++)
		cout<<setprecision(3)<<a[i]<<" ";
}

float findMax(float a[], int l, int r){
	if(l==r)	
		return a[l];
	else {
		int m = (l+r)/2;
		if(findMax(a,l,m) > findMax(a,m+1,r))
			return findMax(a,l,m);
		else
			return findMax(a,m+1,r);
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
	cout<<"\nMax = "<<findMax(a,0,n-1);
	return 0;
}

