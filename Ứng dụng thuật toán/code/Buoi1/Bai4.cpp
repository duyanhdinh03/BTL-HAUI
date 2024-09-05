#include <bits/stdc++.h>
using namespace std;

string peoples[] = {"Tran","Cong","Trung","Binh","Hoang","Mai"};



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




void show(int index[], int n) {
	for(int i = 0 ; i < n ; ++i) {
		cout<<peoples[index[i]]<<" ";
	}
	cout<<"\n";
}

int main() {
	int now[100];
	int n = 6, k=4;
	for(int i = 0; i < k; ++i) {
		now[i] = i;	
	}
	
	do {
		show(now, k);
	} while(toNext(now, n, k));
}
