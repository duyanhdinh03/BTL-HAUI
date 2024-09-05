#include<iostream>

using namespace std;

void merge(int arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++){
    	L[i] = arr[l + i];
	}  
    for (j = 0; j < n2; j++){
    	R[j] = arr[m + 1+ j];
	}
    i = 0;
    j = 0; 
    k = l; 
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){ // < tang , > giam
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int a[], int l, int r){
    if (l < r){
        int m = l+(r-l)/2;
        mergeSort(a, l, m);
        mergeSort(a, m+1, r);
        merge(a, l, m, r);
    }
}

void donuoc(int *a, int n, int k){
	int dem=0;
	int i=0;
	mergeSort(a,0,n-1);
	while(k>0 && i<n){\
		if(a[i]<=k){
			dem++;
			k-=a[i];
			i++;
		} else {
			i++;
		}
	}
	if(dem==0){
		cout<<"Khong co chai nao day!";
	} else {
		cout<<"So chai day: "<<dem;
	}
}

int main(){
	int *a,n,k;
	cin>>n>>k;
	a = new int[n];
	for(int i=0;i<n;i++)
		cin>>a[i];
	donuoc(a,n,k);
}
