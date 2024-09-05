#include<bits/stdc++.h>

using namespace std;

void xuat(float *a,int n){
	for(int i=0;i<n;i++)
		cout<<setprecision(3)<<a[i]<<" ";
}

float TinhTong(float *a, int l, int r){
	if(r==l){
		float s = 0;
		if(a[l]>0)
			s += a[l];
		return s;
	} else {
		return TinhTong(a,l,(l+r)/2) + TinhTong(a,(l+r)/2+1,r);
	}
}

int main(){
	int n=20;
	float *a;
	srand(time(NULL));
	a = new float[n];
	for(int i=0;i<n;i++)
		a[i] = (float) (rand()%100 - rand()%100) / 3 ;
	xuat(a,n);
	cout<<"\nTong cac so duong: "<<TinhTong(a,0,n-1);
}
