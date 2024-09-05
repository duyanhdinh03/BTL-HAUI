#include<bits/stdc++.h>

using namespace std;

struct GoiHang{
	int stt;
	int w; //Trong luong
	int v; //Gia tri
};

vector<int> x;

void xuat(GoiHang *a, int n){
	cout<<"\nThu tu     : ";
	for(int i=0;i<n;i++)
		cout<<a[i].stt<<" ";
	cout<<"\nTrong luong: ";
	for(int i=0;i<n;i++)
		cout<<a[i].w<<" ";
	cout<<"\nGia tri    : ";
	for(int i=0;i<n;i++)
		cout<<a[i].v<<" ";
}

void algo(GoiHang *a, int n, int m, int **f){
	for(int i=0;i<=m;i++)
		f[0][i] = 0;
	for(int i=1;i<=n;i++){
		for(int j=0;j<=m;j++){
			f[i][j] = f[i-1][j];
			int temp = f[i-1][j - a[i-1].w] + a[i-1].v;
			if(a[i-1].w <= j && f[i][j] < temp){
				f[i][j] = temp;
			}
		}
	}
}

void result(GoiHang *a,int **f, int n, int m){
	algo(a,n,m,f);
	cout<<"\nMax value: " << f[n][m] << endl;
	int i = n, j = m;
	while (i != 0) {
		if (f[i][j] != f[i - 1][j]) {
			x.push_back(i);
			j = j - a[i-1].w; //Lay goi thu i
		}
		i--;
	}
}

int main(){
	int n = 6,m;
	GoiHang a[n+1];
	srand(time(NULL));
	for(int i=0;i<n;i++){
		a[i].stt = i+1;
		a[i].w = rand() % 10 + 1;
		a[i].v = rand() % 10 + 1;
	}
	xuat(a,n);
	cout<<"\nNhap m: ";
	cin>>m;
	int **f;
	f = new int*[n+1];
	for(int i=0;i<n+1;i++)
		f[i] = new int[m+1];
	result(a,f,n,m);
	for(int i=0;i<=n;i++){
		for(int j=0;j<=m;j++){
			cout<<f[i][j]<<" ";
		}
		cout<<endl;
	}
	for(int i=x.size()-1;i>=0;i--)
		cout<<x[i]<<" ";
}
