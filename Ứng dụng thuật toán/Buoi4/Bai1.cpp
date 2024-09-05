#include <bits/stdc++.h>

using namespace std;

struct HocSinh{
	char ten[10];
	char hodem[20];
	int namsinh;
	char diachi[20];
};

void quick_sort(HocSinh a[], int left, int right){
	if(left<right){
		int k = (left+right)/2;
		char * t = a[k].ten;
		int i=left, j=right;
		do{
			while(strcmp(a[i].ten, t)<0)
				i++;
			while(strcmp(a[j].ten, t)>0)
				j--;
			if(i<=j){
				swap(a[i], a[j]);
				i++;
				j--;
			}	
		}while(i<=j);
		quick_sort(a,left,j);
		quick_sort(a,i,right);
	}
}

void xuat(HocSinh a[], int n){
	cout<<left<<setw(20)<<"Ten"<<setw(20)<<"Ho Dem"<<setw(20)<<"Nam Sinh"<<setw(20)<<"Dia Chi"<<endl;
	for(int i=0;i<n;i++)
		cout<<setw(20)<<a[i].ten<<setw(20)<<a[i].hodem<<setw(20)<<a[i].namsinh<<setw(20)<<a[i].diachi<<endl;
}

int search(HocSinh a[], char t[], int l, int r){
	int m = (l+r)/2;
	if(strcmp(a[m].ten, t) == 0)
		return m;
	if(l>r)
		return -1;
	if(strcmp(a[m].ten, t) > 0){
		search(a,t,l,m-1);
	} else {
		search(a,t,m+1,r);
	}
}

int main() {
	int n=6;
	HocSinh a[6] = {
		{"Anh", "Nguyen Nam", 2002, "Hung Yen"},
		{"Dai", "Dinh", 2002, "Ninh Binh"},
		{"Duc", "Nguyen", 2002, "Phu Tho"},
		{"Lam", "Tran", 2002, "Ha Noi"},
		{"Hai", "Nguyen", 2001, "Hai Duong"},
		{"Bach", "Le", 2002, "Nghe An"},
	};
	
	cout<<"Dach sach sau khi sap xep theo ten: "<<endl;
	quick_sort(a,0,n-1);
	xuat(a,n);
	
	char t[10];
	cout<<"\nNhap ten muon tim: ";
	fflush(stdin);
	gets(t);
	
	int i = search(a,t,0,n-1);
	if(i==-1){
		cout<<"\nKhong tim thay";
	} else {
		cout<<"\nKet qua tim la:"<<endl;
		cout<<left<<setw(20)<<"Ten"<<setw(20)<<"Ho Dem"<<setw(20)<<"Nam Sinh"<<setw(20)<<"Dia Chi"<<endl;
		cout<<setw(20)<<a[i].ten<<setw(20)<<a[i].hodem<<setw(20)<<a[i].namsinh<<setw(20)<<a[i].diachi<<endl;
	}
	return 0;
}

