#include<bits/stdc++.h>

using namespace std;

struct CongTy{
	char ten[30];
	int soLoai;
	int doanhThu;
	int soNhanVien;
};

int n = 7;
CongTy d[7] = 
{
	{"TNHH a1", 80, 10, 20},
	{"CP a2", 100, 100, 600},
	{"TNHH a3", 100, 10, 100},
	{"TNHH a4", 70, 30, 60},
	{"CP a5", 40, 300, 4},
	{"TNHH a6", 50, 60, 10},
	{"CP a7", 200, 60, 100}
};

void output(CongTy *d, int n){
	cout<<"Ten"<<"\t\t"<<"So loai"<<"\t\t"<<"Doanh thu"<<"\t"<<"So nhan vien"<<endl;
	for(int i=0;i<n;i++)
		cout<<d[i].ten<<"\t\t"<<d[i].soLoai<<"\t\t"<<d[i].doanhThu<<"\t\t"<<d[i].soNhanVien<<endl;
}

int findMin(CongTy *d, int l, int r){
	if(l==r)
		return l;
	else {
		int m = (l+r)/2;
		if(d[findMin(d,l,m)].soLoai < d[findMin(d,m+1,r)].soLoai)
			return findMin(d,l,m);
		else
			return findMin(d,m+1,r);
	}
}

int N = 200;
int F[7+1][200+1];
vector<int> vec;

void algorithm(){
	int m = N;
	for(int i=0;i<=m;i++)
		F[0][i] = 0;
	for(int i=1;i<=n;i++){
		for(int j=0;j<=m;j++){
			F[i][j] = F[i-1][j];
			int temp = F[i-1][j - d[i-1].soNhanVien] + d[i-1].doanhThu;
			if(d[i-1].soNhanVien <= j && F[i][j] < temp)
				F[i][j] = temp;
		}
	}
}

void result(){
	cout<<"Tong doanh thu lon nhat: "<<F[n][N]<<endl;
	int i=n,j=N;
	while(i != 0){
		if(F[i][j] != F[i-1][j]){
			vec.push_back(i);
			j = j - d[i].soNhanVien;
		}
		i--;
	}
	cout<<"Cac cong ty la: ";
	for(int i=vec.size()-1;i>=0;i--)
		cout<<d[vec[i]-1].ten<<" ";
}

char *T;
char P[] = "TNHH";
int dem = 0;

int char_in_string(char k, char a[]){
	for(int i=0; i<strlen(a); i++){
		if(a[i]==k)
			return i;
	}
	return -1;
}

void Boyer_Moore_Horspool(){
	int i = strlen(P), v = strlen(P);
	while(i < strlen(T)){
		int x = v - 1, j = i - 1;
		while(T[j] == P[x] && x > -1){
			j--;
			x--;
		}
		if(x < 0){
			dem++;
			i = i + v;
		} else {
			x = char_in_string(T[j], P);
			if(x < 0)
				i = i + v;
			else
				i = i + v - x - 1;
		}
	}
}

int main(){
	cout<<"Danh sach cong ty: \n";
	output(d,n);
	int vtMin = findMin(d,0,n-1);
	cout<<"\nCong ty san xuat it hang hoa nhat la: \n";
	cout<<d[vtMin].ten<<"\t\t"<<d[vtMin].soLoai<<"\t\t"<<d[vtMin].doanhThu<<"\t\t"<<d[vtMin].soNhanVien<<endl;
	algorithm();
	result();
	
	for(int i=0;i<n;i++){
		T = d[i].ten;
		Boyer_Moore_Horspool();
	}
	if(dem != 0){
		cout<<"\nTNHH xuat hien "<<dem<<" lan"<<endl;
	} else {
		cout<<"\nTNHH khong xuat hien"<<endl;
	}
	return 0;
}

