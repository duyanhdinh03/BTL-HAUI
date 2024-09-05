#include<bits/stdc++.h>

using namespace std;

struct HocSinh{
	char ten[20];
	int tuoi;
	float diemTK;
};

int n = 7;
HocSinh d[7] = 
{
	{"Lan Anh", 20, 9.1},
	{"Nam", 21, 8},
	{"Duc", 20, 4},
	{"Nam", 21, 6.5},
	{"Nam", 20, 7.4},
	{"Nam", 21, 9.8},
	{"Lan Anh", 19, 3}
};

void Output(HocSinh *d, int n){
	cout<<"HoTen\t\t"<<"Tuoi\t\t"<<"DiemTK\n";
	for(int i=0;i<n;i++)
		cout<<d[i].ten<<"\t\t"<<d[i].tuoi<<"\t\t"<<d[i].diemTK<<endl;
}

int findMax(HocSinh *d, int l, int r){
	if(l>=r){
		return l;
	} else {
		int m = (l+r)/2;
		if(d[findMax(d,l,m)].diemTK >= d[findMax(d,m+1,r)].diemTK){
			return findMax(d,l,m);
		} else {
			return findMax(d,m+1,r);
		}
	}
}

char *A;
char *B;
int **L;

int algorithm(int m, int n){
	for(int i=0;i<=m;i++)
		L[i][0] = 0;
	for(int j=0;j<=n;j++)
		L[0][j] = 0;
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++){
			if(A[i-1] == B[j-1]){
				L[i][j] = L[i-1][j-1] + 1;
			} else {
				L[i][j] = (L[i-1][j] > L[i][j-1]) ? L[i-1][j] : L[i][j-1];
			}
		}
	}
	return L[m][n];
}

void xau_con(int m, int n, char *ss){
	int i=m,j=n,k=0;
	while(L[i][j] != 0){
		if(A[i-1] == B[j-1]){
			ss[k] = A[i-1];
			k++;
			i--;
			j--;
		} else {
			if(L[i-1][j] > L[i][j-1]){
				i--;
			}else{
				j--;
			}
		}
	}
	ss[k] = '\0';
	strrev(ss);
}

char s[100] = {};
int *z;

void z_algo(char *s, int *z){
	int n = strlen(s), l=0, r=0;
	for(int i=1;i<n;i++){
		if(l > r){
			l = r = i;
			while(r < n && s[r-l] == s[r]){
				r++;
			}
			z[i] = r - l;
			r--;
		} else if(z[i-l] < r-i+1){
			z[i] = z[i-l];
		} else {
			l = i;
			while(r < n && s[r-l] == s[r]){
				r++;
			}
			z[i] = r - l;
			r--;
		}
	}
}

char *k;
char p[100];

void resultZ(){
	for(int i=0;i<n;i++){
		strcpy(p,p);
		strcat(p,",");
		strcat(p,d[i].ten);
	}
	int q[7] = {0,0,0,0,0,0,0};
	for(int i=0;i<7;i++){
		k = d[i].ten;
		strcpy(s,k);
		strcat(s,"&");
		strcat(s,p);
		cout<<s<<endl;
		z = new int[strlen(s)];
		for(int e=0;e<strlen(s);e++)
			z[e] = 0;
		z_algo(s,z);
		for(int e=0;e<strlen(s);e++)
			cout<<z[e];
			cout<<endl;
		for(int e=0;e<strlen(s);e++){
			if(z[e] == strlen(k)){
				q[i] ++;
			}
		}
		for(int e=0;e<7;e++)
		cout<<q[e];
		cout<<endl;
	}
	
}


int main(){
	cout<<"Danh sach hoc sinh: "<<endl;
	Output(d,n);
	int vtMax = findMax(d,0,n-1);
	cout<<"\nHoc sinh co diem tong ket lon nhat la: \n";
	cout<<d[vtMax].ten<<"\t\t"<<d[vtMax].tuoi<<"\t\t"<<d[vtMax].diemTK<<endl;
	A = d[0].ten;
	B = d[n-1].ten;
	int m = strlen(A);
	int k = strlen(B);
	L = new int*[m+1];
	for(int i=0;i<=m;i++)
		L[i] = new int[k+1];
	cout<<"\nDo dai xau con chung dai nhat: "<<algorithm(m,k)<<endl;
	char ss[255];
	xau_con(m,k,ss);
	cout<<"Xau con chung la: "<<ss<<endl;
	resultZ();
	return 0;
}

