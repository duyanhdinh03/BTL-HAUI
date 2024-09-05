#include<bits/stdc++.h>

using namespace std;

#define MAX 9

void inmt(int a[][MAX], int n){
	for(int i=0; i<n; i++){
		for( int j=0; j<n; j++){
			cout.width(4);
			cout<<a[i][j];
		}
		cout<<endl;
	}
}

int timduong(int a[][MAX], int n, int start, int end, int path[], int &np){
	int mark[MAX];
	int cur = start;
	int next;
	int tongcp = 0;
	path[np++] = start;
	for(int i=0; i<n; i++)
		mark[i] = 0;
	mark[start] = 1;
	
	do{
		int i = 0;
		while (i<n && (a[cur][i] == 0 || mark[i] == 1))
			i++;
		next = i;
		for(int j=0;j<n;j++)
			if(mark[j] == 0 && a[cur][j] < a[cur][next] && a[cur][j] != 0)
				next = j;
		path[np++] = next;
		mark[next] = 1;
		tongcp += a[cur][next];
		cur = next;
		if(next == end)
			return tongcp;
	}while(next != -1);	
}

int main(){
	int n=9;
	char dinh[n] = {'A','B','C','D','E','F','G','H','I'};
	int a[MAX][MAX] = { {0 ,0 ,16,10,19,0 ,0 ,0 ,0 },
				    	{0 ,0 ,0 ,0 ,0 ,3 ,8 ,12,5 },
				    	{16,0 ,0 ,18,0 ,0 ,0 ,20,0 },
				    	{10,0 ,18,0 ,8 ,7 ,8 ,0 ,0 },
				    	{19,0 ,0 ,8 ,0 ,0 ,0 ,0 ,9 },
						{0 ,3 ,0 ,7 ,0 ,0 ,5 ,15,0 },
						{0 ,8 ,0 ,8 ,0 ,5 ,0 ,0 ,4 },
						{0 ,12,20,0 ,0 ,15,0 ,0 ,0 },
						{0 ,5 ,0 ,0 ,9 ,0 ,4 ,0 ,0 }};
	inmt(a,n);
	int path[MAX];
	int tongcp;
	int np=0;
	tongcp = timduong(a,n,0,1,path,np);
	if(tongcp == 0)
		cout<<"\nKhong tim duoc duong di!";
	else {
		cout<<"\nTim duoc duong di: \n";
		cout<<dinh[path[0]];
		for(int i=1;i<np;i++)
			cout<<"-->"<<dinh[path[i]];
		cout<<"\nTong chi phi: "<<tongcp;
	}
}
