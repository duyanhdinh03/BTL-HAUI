#include<bits/stdc++.h>

using namespace std;

void heapify(float *a, float *b, int i, int n){
	int L = 2*i+1;
	int R = 2*i+2;
	int max = i;
	if(L <= n-1 && a[L] > a[i]) // > tang, < giam
		max = L;
	if(R <= n-1 && a[R] > a[max]) // > tang, < giam
		max = R;
	if(max != i){
		swap(a[i], a[max]);
		swap(b[i], b[max]);
		heapify(a, b, max, n);
	}
}

void buildHeap(float *a, float *b, int n){
	for(int i=n/2-1; i>=0; i--){
		heapify(a,b,i,n);
	}
}

void heap_sort(float *a, float *b, int n){
	buildHeap(a,b,n);
	for(int i=n; i>1; i--){
		swap(a[0], a[i-1]);
		swap(b[0], b[i-1]);
		heapify(a,b,0,i-1);
	}
}

void laplich(float *a, float *b, int *kq, int n){
	heap_sort(b,a,n);
	float last_finish = 0;
	for(int i=0;i<n;i++){
		if(a[i] >= last_finish){
			kq[i] = 1;
			last_finish = b[i];
		}
	}
}

void showlich(float *a, float *b, int *kq, int n){
	for(int i=0;i<n;i++)
		cout<<kq[i]<<" ";
	cout<<endl;
	cout<<left<<setw(10)<<"Bat dau"<<setw(10)<<"Ket thuc"<<endl;
	for(int i=0;i<n;i++){
		if(kq[i]==1){
			cout<<setw(10)<<a[i]<<setw(10)<<b[i]<<endl;
		}
	}
}

int main(){
	float *a, *b;
	int *kq, n;
	cin>>n;
	a = new float[n];
	b = new float[n];
	kq = new int[n];
	for(int i=0;i<n;i++)
		cin>>a[i];
	for(int i=0;i<n;i++)
		cin>>b[i];
	for(int i=0;i<n;i++)
		kq[i]=0;
	laplich(a,b,kq,n);
	showlich(a,b,kq,n);
}
