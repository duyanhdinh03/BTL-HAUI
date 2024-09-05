#include<bits/stdc++.h>

using namespace std;

bool flag[100][100];
int a[100][100];

void loang(int i, int j){
	flag[i][j] = true;
	if(a[i-1][j]==a[i][j] && !flag[i-1][j])
		loang(i-1,j);
	if(a[i+1][j]==a[i][j] && !flag[i+1][j])
		loang(i+1,j);
	if(a[i][j-1]==a[i][j] && !flag[i][j-1])
		loang(i,j-1);
	if(a[i][j+1]==a[i][j] && !flag[i][j+1])
		loang(i,j+1);
}

void show(int n, int m){
	for(int i = 1 ; i <= n ; ++i){
		for(int j = 1 ; j <= m ; ++j){
			cout<<flag[i][j]<<" ";
		}
		cout<<endl;
	}	
}

int main(){
	int n,m;
	cin >> n >> m;
    for(int i = 1 ; i <= n ; i++)
    	for(int j = 1 ; j <= m ; j++) 
			cin >> a[i][j];
	int dem=0;
	cout<<endl;
    for(int i = 1 ; i <= n ; i++)
    	for(int j = 1 ; j <= m ; j++){
    		if(a[i][j] == 1 && !flag[i][j]) {
    			loang(i,j);	
    			show(n, m);
    			cout<<endl;
    			dem++;
			}	
		}
	
    cout<<"So mien lien thong: "<<dem;		
}
