#include<bits/stdc++.h>

using namespace std;

void quickSort(int *a, int left, int right){
	if(left<right){
		int k = (left+right)/2;
		int t = a[k];
		int i = left, j = right;
		do{
			while(a[i] > t)
				i++;
			while(a[j] < t)
				j--;
			if(i<=j){
				swap(a[i], a[j]);
				i++;
				j--;
			}
		}while(i<=j);
		quickSort(a,left,j);
		quickSort(a,i,right);
	}
}

bool ThamLam(int *a, int n, int m, int *b, int &j){
	quickSort(a,0,n-1);
	int dem=0;
	int i=0;
	while(m >= a[i]){
		b[j] = a[i];
		dem++;
		m -= a[i];
		i++;
		j++;
	}
	if(m>0){
		b[j] = a[i];
		dem++;
		m -= a[i];
		i++;
		j++;
	}
	if(dem==0){
		return false;
	} else {
		return true;
	}
}

int main(){
	int n = 6, j=0;
	srand(time(NULL));
	int m = rand() % 100 + n*2 * 10;
	int *a = new int[n];
	int *b = new int[n];
	for(int i=0;i<n;i++)
		a[i] = rand() % 100 + 1;
	cout<<"Tai trong cac xe tai"<<endl;
	for(int i=0;i<n;i++)
		cout<<a[i]<<" ";
	cout<<"\nTan hang van chuyuen: \n"<<m<<endl;
	
	if(ThamLam(a,n,m,b,j)){
		cout<<"Cac xe van chuyen\n";
		for(int i=0;i<j;i++){
			cout<<b[i]<<" ";
		}
	} else {
		cout<<"Khong co xe nao!";
	}
}
