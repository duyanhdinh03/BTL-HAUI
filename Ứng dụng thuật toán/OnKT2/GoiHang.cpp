#include<bits/stdc++.h>

using namespace std;

int n = 6;
struct GoiHang{
	char maGoi[10];
	int khoiLuong;
	int giaTien;
};

GoiHang d[6] =
{
	{"C001", 20, 2000},
	{"D001", 110, 11000},
	{"E002", 70, 12000},
	{"F001", 80, 6000},
	{"H001", 40, 7000},
	{"H003", 200, 6600}
};

void Output(GoiHang *d, int n){
	for(int i=0;i<n;i++){
		cout<<d[i].maGoi<<"\t\t"<<d[i].khoiLuong<<"\t\t"<<d[i].giaTien<<endl;
	}
}

int timViTri(GoiHang *d, char x[], int l, int r){
	int m = (l+r)/2;
	if(strcmp(d[m].maGoi, x) == 0)
		return m;
	if(l>r)
		return -1;
	if(strcmp(d[m].maGoi, x) > 0) {
		timViTri(d,x,l,m-1);
	} else {
		timViTri(d,x,m+1,r);
	}
}

int m = 100;
int F[7][101];
vector<int> vec;

void algorithm(){
	for(int i=0;i<=m;i++)
		F[0][i] = 0;
	for(int i=1;i<=n;i++){
		for(int j=0;j<=m;j++){
			F[i][j] = F[i-1][j];
			int temp = F[i-1][j - d[i-1].khoiLuong] + d[i-1].giaTien;
			if(d[i-1].khoiLuong <= j && F[i][j] < temp)
				F[i][j] = temp;
		}
	}
}

void result(){
	cout<<"Gia tien max: "<<F[n][m]<<endl;
	int i=n,j=m;
	while(i != 0){
		if(F[i][j] != F[i-1][j]){
			vec.push_back(i);
			j = j - d[i-1].khoiLuong;
		}
		i--;
	}
	cout<<"Cac goi hang lay duoc la: ";
	for(int i=vec.size()-1;i>=0;i--)
		cout<<d[vec[i]-1].maGoi<<" ";
}

char *T;
char P[] = "H00";
int dem = 0;

int char_in_string(char k, char a[]){
	for(int i=0;i<strlen(a);i++){
		if(a[i] == k)
			return i;
	}
	return -1;
}

void Boyer_Moore_Horsepool(){
	int i=strlen(P), v = strlen(P);
	while(i<strlen(T)){
		int x = v - 1, j = i - 1;
		while(T[j] == P[x] && x > -1){
			j--;
			x--;
		}
		if(x<0){
			dem++;
			i = i + v;
		} else {
			x = char_in_string(T[j], P);
			if(x<0)
				i = i + v;
			else
				i = i + v - x - 1;
		}
	}
}

int main(){
	cout<<"Danh sach goi hang: "<<endl;
	Output(d,n);
	cout<<endl;
	char x[] = "D001";
	int vt = timViTri(d,x,0,n-1);
	if(vt == -1){
		for(int i=0;i<strlen(x);i++)
			cout<<x[i];
		cout<<" khong tim thay vi tri!"<<endl;
	} else {
		for(int i=0;i<strlen(x);i++)
			cout<<x[i];
		cout<<" co vi tri la: "<<vt+1<<endl;
	}
	algorithm();
	result();
	cout<<endl;
	
	for(int i=0;i<n;i++){
		T = d[i].maGoi;
		Boyer_Moore_Horsepool();
	}
	if(dem>0){
		cout<<"H00 xuat hien "<<dem<<" lan"<<endl;
	}else{
		cout<<"H00 khong xuat hien"<<endl;
	}
	return 0;
}
