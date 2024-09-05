#include <iostream>
using namespace std;

int tongCacChuSo(int n) {
	if(n<10) {
		return n;
	}
	return n % 10 + tongCacChuSo(n/10);
}

int main() {
	int n;
	cin>>n;
	cout<<tongCacChuSo(n);
}
