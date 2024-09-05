#include<bits/stdc++.h>

using namespace std;

struct SanPham{
	char id[10];
	char ten[20];
	int khoiLuong;
	int giaTri;
};

int n = 8;
SanPham d[8]=
{
	{"SP0001", "a 123456789", 10, 40},
	{"SP0002", "b 2223333", 100, 200},
	{"SP0003", "c 0123456789", 80, 50},
	{"SP0004", "d 2222212", 10, 70},
	{"SP0005", "e 0123456789", 10, 20},
	{"SP0006", "tivi 123", 50, 20},
	{"SP0008", "tivi 245", 100, 30},
	{"SP0088", "z 0123456789", 30, 20}
};

void output(SanPham *d, int n){
	cout<<"Ma SP"<<"\t\t"<<"Ten SP"<<"\t\t\t"<<"Khoi luong"<<"\t"<<"Gia tri"<<endl;
	for(int i=0;i<n;i++)
		cout<<d[i].id<<"\t\t"<<d[i].ten<<"\t\t"<<d[i].khoiLuong<<"\t\t"<<d[i].giaTri<<endl;
}

int findViTri(SanPham *d, SanPham x, int l, int r){
	if(l < r){
		int m = (l+r)/2;
		if(strcmp(d[m].ten, x.ten) >= 0)
			return findViTri(d,x,l,m-1);
		else
			return findViTri(d,x,m+1,r);
	} else {
		return l;
	}
}

char *A;
char *B;
int **L;

int algorithm(int m, int n){
	for(int i=0; i<=m; i++)
		L[i][0] = 0;
	for(int j=0; j<=n; j++)
		L[0][j] = 0;
	for(int i=1; i<=m; i++){
		for(int j=1; j<=n; j++){
			if(A[i-1] == B[j-1])
				L[i][j] = L[i-1][j-1] + 1;
			else
				L[i][j] = (L[i-1][j] > L[i][j-1]) ? L[i-1][j] : L[i][j-1];
		}
	}
	return L[m][n];
}

void xau_con(int m, int n, char *ss){
	int i=m, j=n, k=0;
	while(L[i][j] != 0){
		if(A[i-1] == B[j-1]){
			ss[k] = A[i-1];
			k++;
			i--;
			j--;
		} else {
			if(L[i-1][j] > L[i][j-1])
				i--;
			else
				j--;
		}
	}
	ss[k] = '\0';
	strrev(ss);
}

void result(){
	int check=0;
	for(int i=0;i<n;i++){
		if(i==7){
			continue;
		}
		A = d[7].ten;
		B = d[i].ten;
		int m = strlen(A);
		int k = strlen(B);
		L = new int*[m+1];
		for(int r=0;r<=m;r++)
			L[r] = new int[k+1];
		int c=algorithm(m,k);
		char ss[255];
		xau_con(m,k,ss);
		if(strlen(ss)>=10){
			cout<<"\nTen con chung voi san pham SP0088 la san pham ten : "<<B<<" voi sau con chung la :"<<ss<<" voi "<<c<< " ki tu giong nhau";
			check++;
		}
	}
	if(check == 0){
		cout<<"\nSan pham SP0088 khong co ten con chung";
	}
}

char *T;
char P[] = "tivi";
int dem=0;

int char_in_string(char k, char a[]){
	for(int i=0; i<strlen(a); i++){
		if(a[i] == k)
			return i;
	}
	return -1;
}

void Boyer_Moore_Horspool(){
	int i=strlen(P), v=strlen(P);
	while(i<strlen(T)){
		int x=v-1, j=i-1;
		while(T[j] == P[x] && x > -1){
			j--;
			x--;
		}
		if(x < 0){
			dem++;
			i=i+v;
		} else {
			x = char_in_string(T[j], P);
			if(x < 0)
				i=i+v;
			else
				i=i+v-x-1;
		}
	}
}

int main(){
	cout<<"Cac san pham: "<<endl;
	output(d,n);
	SanPham x = {"SPnew", "f 111", 50, 50};
	int kq = findViTri(d,x,0,n-1);
	cout<<"\nVi tri thich hop de chen la: "<<kq+1<<endl;;
	result();

	for(int i=0;i<n;i++){
		T = d[i].ten;
		Boyer_Moore_Horspool();
	}
	if(dem>0){
		cout<<"\nDanh sach co san pham tivi";
	}else{
		cout<<"\nDanh sach khong co san pham tivi";
	}
	
	return 0;
}

