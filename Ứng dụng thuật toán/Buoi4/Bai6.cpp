#include <bits/stdc++.h>

using namespace std;

void xuat(int *a,int n){
	for(int i=0;i<n;i++)
		cout<<a[i]<<" ";
}

int findMinEven(int a[], int l, int r){
	if(l==r){
		return a[l];
	} else {
		int m = (l+r)/2;
		int minL = findMinEven(a,l,m);
		int minR = findMinEven(a,m+1,r);
		if(minL < minR && minL % 2 == 0)
			return minL;
		else if(minL > minR && minR % 2 == 0)
			return minR;
		else if(minL % 2 == 0)
			return minL;
		else if(minR % 2 == 0)
			return minR;
		else 
			return -1;
	}
}

int main() {
	int n=20;
	int *a;
	srand(time(NULL));
	a = new int[n];
	for(int i=0;i<n;i++)
		a[i] = rand()%100;
	xuat(a,n);
	if(findMinEven(a,0,n-1)==-1)
		cout<<"\nMang khong co so chan!";
	else
		cout<<"\nMin Even:  = "<<findMinEven(a,0,n-1);
	return 0;
}
