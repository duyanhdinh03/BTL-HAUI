#include <bits/stdc++.h>
using namespace std;

bool toNext(int now[], int n, int k) {
	int i = k - 1;
	while(i>=0) {
		if(now[i] < n - k + i)
			break;
		--i;
	}
	if(i==-1)
		return false;
	now[i]++;
	i++;
	while(i<k) {
		now[i] = now[i-1] + 1;
		i++;
	}
	return true;

}


void show(int now[], int k) {
	for(int i = 0 ; i < k ; ++i) {
		cout<<now[i]+1<<" ";
	}
	cout<<"\n";
}


int main() {
	int now[100];
	int n, k;
	cin>>n>>k;
	for(int i = 0; i < k; ++i) {
		now[i] = i;	
	}
	
	do {
		show(now, k);
	} while(toNext(now, n, k));
}
