#include<bits/stdc++.h>

using namespace std;

struct Hang{
	int kichThuoc;
	int khoiLuong;
};

void show(Hang *a, int n){
	cout<<"Khoi Luong: ";
	for(int i=0;i<n;i++)
		cout<<a[i].khoiLuong<<" ";
	cout<<"\nKich thuoc: ";
	for(int i=0;i<n;i++)
		cout<<a[i].kichThuoc<<" ";
	cout<<endl;
}

void quickSort(Hang *a, int left, int right){
	if(left<right){
		int k = (left+right)/2;
		int t = a[k].khoiLuong / a[k].kichThuoc;
		int i=left, j=right;
		do{
			while(a[i].khoiLuong / a[i].kichThuoc < t)
				i++;
			while(a[j].khoiLuong / a[j].kichThuoc > t)
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

bool ThamLam(Hang *a, int n, int k, Hang *b, int &j, int step){
	quickSort(a,0,n-1);
	int i=step, dem=0, m = k;
	while(m>=0 && i<n){
		if(a[i].kichThuoc <= m){
			b[j] = a[i];
			dem++;
			m -= a[i].kichThuoc;
			i++;
			j++;
		}else {
			i++;
		}
	}
	if(dem!=0)
		return true;
	else {
		return false;
	}
}

int main(){
	int n=5, j=0;
	srand(time(NULL));
	Hang *a = new Hang[n];
	Hang *b = new Hang[n];
	for(int i=0; i<n; i++){
		a[i].kichThuoc = rand() % 100 + 1;
		a[i].khoiLuong = rand() % 100 + 1;
	}
	int k = rand() % 100 + 1;
	cout<<"Khoi tao: "<<endl;
	show(a,n);
	cout<<"Balo: "<<k<<endl;
	if(ThamLam(a,n,k,b,j,0)){
		cout<<"\nHang trom duoc vao tui: \n";
		show(b,j);
	}else{
		cout<<"\nKhong trom duoc hang nao!";
	}
}
