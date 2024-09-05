#include<bits/stdc++.h>

using namespace std;

int n = 5, m = 4;
int f[100][100];
int a[100];
int temp=0;

int algo(){
	f[0][0] = 1;
	for(int i=1;i<=n;i++)
		f[0][i] = 0;
	for(int i=1;i<=m;i++){
		for(int j=0;j<=n;j++){
			if(j<i){
				f[i][j] = f[i-1][j];
			} else {
				f[i][j] = f[i-1][j] + f[i][j-i];
			}
		}
	}
	return f[m][n];
}

void show(int a[], int n){
	for(int i=1;i<=n;i++)
		cout<<a[i];
	cout<<endl;
}

int Check(int j,int i)
{
    if(j>=a[i-1])return 1;
    return 0;
}
void Try(int i)
{
    int j;
    for(j=1;j<=m;j++){
    	if (Check(j,i))
        { 
            a[i]=j;                    
            temp+=j; 
            if(temp==n) {
            	show(a,i);
		    }
            else if(temp<n)Try(i+1); 
            temp-=j;         
        } 
     }
}

int main(){
	cout<<algo()<<endl;
	Try(1);
}
