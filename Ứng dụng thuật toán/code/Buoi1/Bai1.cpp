#include <bits/stdc++.h>
using namespace std;
void sinhChuoiNhiPhanDoDai(int n, string s="") {
	if(s.size()==n)
		cout<<s+"\n";
	else {
		sinhChuoiNhiPhanDoDai(n, s+"0");
		sinhChuoiNhiPhanDoDai(n,s+"1");
	}
}
int main() {
	sinhChuoiNhiPhanDoDai(3);
}
