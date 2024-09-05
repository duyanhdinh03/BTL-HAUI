#include<bits/stdc++.h>

using namespace std;

struct HocSinh{
	char hoTen[20];
	int tuoi;
	float diemTK;
};

int n = 7;
HocSinh d[7] = 
{
	{"Nam Anh", 20, 9},
	{"Duc", 21, 8},
	{"Nam", 20, 4},
	{"Hai", 21, 6},
	{"Nam", 20, 7},
	{"Nam", 21, 5},
	{"Anh", 19, 3}
};

void Output(HocSinh *d, int n){
	for(int i=0;i<n;i++)
		cout<<d[i].hoTen<<"\t\t"<<d[i].tuoi<<"\t\t"<<d[i].diemTK<<endl;
}

int findMax(HocSinh *d, int l, int r){
	if(l==r)
		return l;
	else {
		int m = (l+r)/2;
		if(d[findMax(d,l,m)].diemTK > d[findMax(d,m+1,r)].diemTK){
			return findMax(d,l,m);
		}else{
			return findMax(d,m+1,r);
		}
	}
}

char *A;
char *B;
int **L;

int algo(int m, int n){
	for(int i=0; i<=m;i++)
		L[i][0] = 0;
	for(int j=0;j<=n;j++)
		L[0][j] = 0;
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++){
			if(A[i-1] == B[j-1])
				L[i][j] = L[i-1][j-1] + 1;
			else
				L[i][j] = (L[i-1][j] > L[i][j-1]) ? L[i-1][j] : L[i][j-1];
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
		}else{
			if(L[i-1][j] > L[i][j-1])
				i--;
			else
				j--;
		}
	}
	ss[k] = '\0';
	strrev(ss);
}

char s[100];
void z_algo(char *s, int *z){
	int n = strlen(s), l = 0, r = 0;
	for(int i=1;i<n;i++){
		if(l>r){
			l = r = i;
			while(r<n && s[r-l] == s[r])
				r++;
			z[i] = r - l;
			r--;
		} else if(z[i-l] < r-i+1)
			z[i] = z[i-l];
		else{
			l = i;
			while(r<n && s[r-l] == s[r]) {
				r++;
			}
			z[i] = r - l;
			r--;
		}
	}
}

char *k;
char p[50];
int q[50] = {0,0,0,0,0,0,0};
void InZ(){
	for(int i=0;i<7;i++){
		strcpy(p,p);
		strcat(p, d[i].hoTen);
	}
	int max = q[0];
	for(int i=0;i<7;i++){
		k = d[i].hoTen;
		strcpy(s,k);
		strcat(s, "&");
		strcat(s, p);
		int *z = new int[strlen(s)];
		z_algo(s,z);
		for(int j=0;j<strlen(s);j++){
			if(z[j] == strlen(k)){
				q[i] ++;
			}
			if(q[i]>max){
				max = q[i];
			}
		}
	}
	int t = 0;
	cout<<endl;
	for(int i=0;i<7;i++){
		if(q[i]>t)
			t = i;
	}
	cout<<"Ten xuat nhien nhieu nhat: "<<d[t].hoTen<<" voi so lan la : "<<max<< " lan" <<endl;
}

int main(){
	cout<<"Danh sach hoc sinh: "<<endl;
	Output(d,n);
	cout<<endl;
	int vtMax = findMax(d,0,n-1);
	cout<<"\nHoc vien co diem tong ket max la: "<<endl;
	cout<<d[vtMax].hoTen<<"\t\t"<<d[vtMax].tuoi<<"\t\t"<<d[vtMax].diemTK<<endl;
	
	A = d[0].hoTen;
	B = d[n-1].hoTen;
	int m = strlen(A);
	int k = strlen(B);
	L = new int*[m+1];
	for(int i=0;i<=m;i++)
		L[i] = new int[k+1]; 
	cout<<"Do dai xau con: "<<  algo(m,k)<<endl;
	char ss[255];
	xau_con(m,k,ss);
	cout<<"Xau con chung dai nhat: "<<ss<<endl;
	
	InZ();
	return 0;
}

