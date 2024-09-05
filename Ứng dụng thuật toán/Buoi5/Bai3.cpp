#include<bits/stdc++.h>
#define inf int(1e9)
#define N 1000

using namespace std;

float a[N+2];
int L[N+2], vet[N+2];
int n = 10;
vector<float> x;

void algo(){
	L[0] = 1, L[1] = 2;
	for(int i=2;i<=n+1;i++){
		for(int j=0;j<i;j++){
			if(a[j] < a[i] && L[i] < L[j] + 1) {
				L[i] = L[j] + 1;
				vet[i] = j;
			}
		}
	}
}

int main(){
	srand(time(NULL));
	a[0] = -inf, a[n+1] = inf;
	for(int i=1;i<=n;i++)
		a[i] = float(rand() % 10 + 1)/1.1;
	for(int i=1;i<=n;i++)
		cout<<a[i]<<" ";
	cout<<endl;
	algo();
	for(int i=n+1;i>0;i=vet[i])
		x.push_back(a[i]);
	cout<<"\nDo dai day con la: "<<x.size()-1<<endl;
	for(int i=x.size()-1;i>0;i--)
		cout<<x[i]<<" ";
}
