#include<bits/stdc++.h>

using namespace std;

struct KienHang{
	int khoiLuong;
	int kichThuoc;
};

void show(KienHang *a, int n){
	cout<<"Khoi Luong: ";
	for(int i=0;i<n;i++)
		cout<<a[i].khoiLuong<<" ";
	cout<<"\nKich thuoc: ";
	for(int i=0;i<n;i++)
		cout<<a[i].kichThuoc<<" ";
	cout<<endl;
}

void quickSort(KienHang *a, int left, int right){
	if(left<right){
		int k = (left+right)/2;
		int t = a[k].khoiLuong / a[k].kichThuoc;
		int i=left, j=right;
		do{
			while(a[i].khoiLuong / a[i].kichThuoc > t)
				i++;
			while(a[j].khoiLuong / a[j].kichThuoc < t)
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

bool ThamLam(KienHang *a, int n, int m, KienHang *b, int &j){
	quickSort(a,0,n-1);
	int i=0, dem=0;
	while(m>=0 && i<n){
		if(a[i].kichThuoc <= m){
			b[j] = a[i];
			dem++;
			m -= a[i].kichThuoc;
			i++;
			j++;
		} else {
			i++;
		}	
	}
	if(dem==0)
		return false;
	else
		return true;
}

int main(){
	int n=5, j=0;
	srand(time(NULL));
	KienHang *a = new KienHang[n];
	KienHang *b = new KienHang[n];
	for(int i=0; i<n; i++){
		a[i].khoiLuong = rand() % 100 + 1;
		a[i].kichThuoc = rand() % 100 + 1;
	}
	int m = rand() % 100 + 1;
	cout<<"Khoi tao: "<<endl;
	show(a,n);
	cout<<"Thung xe: "<<m<<endl;
	
	quickSort(a,0,n-1);
	show(a,n);
	
	if(ThamLam(a,n,m,b,j)){
		cout<<"\nKien hang cho duoc len xe: \n";
		show(b,j);
	}else{
		cout<<"\nKhong co kien hang nao!";
	}
}
