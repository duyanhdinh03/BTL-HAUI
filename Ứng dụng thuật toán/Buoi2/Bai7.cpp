#include<bits/stdc++.h>
#define M 8

using namespace std;

int a[M][M];
int X[8] = {-1, 1, 2, 2, 1, -1, -2, -2};
int Y[8] = {2, 2, 1, -1, -2, -2, -1, 1};
int n;

void init(){
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			a[i][j] = 0;
}

int check(int i, int j){
	return (i>=0 && i<n && j>=0 && j<n);
}

void show(){
	for (int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cout<<left<<setw(4)<<a[i][j];
		}
		cout<<endl;
	}
}

void Try(int dem, int i, int j, int &kt){
	int inext, jnext;
	for(int m=0;m<8;m++) {
		inext = i + X[m];
		jnext = j + Y[m];
		if(check(inext,jnext) && a[inext][jnext]==0){
			a[inext][jnext] = dem+1;
			if(dem == n*n - 1){
				kt = 1;
			} else {
				Try(dem+1,inext,jnext,kt);
				if(!kt)
					a[inext][jnext] = 0;
			}
		}
	}
}

int main(){
	int x, y, kt = 0;
	cout << "Nhap n: ";
    cin >> n;
    init();
    cout << "Nhap vi tri ban dau.\nx: ";
    cin>>x;
    cout << "y: ";
    cin >> y;
    a[x-1][y-1] = 1;
    Try(1, x-1, y-1, kt);
    if(kt){
    	cout<<"\nCac buoc di: \n";
    	show();
	} else {
		cout<<"Khong tim thay duong di.";
	}	
}
