#include <iostream>
using namespace std;

string peoples[] = {"Tran","Cong","Trung","Binh","Hoang","Mai"};

bool toNext(int now[], int n) {
	int i = n - 2;
	while(i>=0) {
		if(now[i]< now[i+1])
			break;
		i--;
	}
	if(i<=-1)
		return false;
	int j = n - 1 ;
	while(now[j] < now[i]) {
		j--;
	}
	swap(now[i], now[j]);
	int l = i + 1, r = n - 1;
	while(l<r) {
		swap(now[l], now[r]);
		l++;
		r--;
	}
	return true;
}


void show(int now[], int n) {
	for(int i = 0; i < n; ++i) {
		cout<<peoples[now[i]]<<" ";
	}
	cout<<"\n";
}

int main() {
	int n = 6;
	int now[n];
	for(int i = 0 ; i < n; ++i) {
		now[i] = i;
	}
	do {
		show(now, n);
	} while(toNext(now,n));
	
}
