#include <bits/stdc++.h>
using namespace std;

void sinhChuoiAB(int n, string s="") {
	if(s.size()==n) {
		cout<<s+"\n";
	}
	else {
		sinhChuoiAB(n, s+"a");
		sinhChuoiAB(n, s+"b");
	}
}

int main() {
	sinhChuoiAB(5);
}

