#include <iostream>
using namespace std;
void hanoiTower(char start, char end, char temp, int n) {
	if(n==1) {
		cout<<start<<"-->"<<end<<"\n";
	}
	else {
		hanoiTower(start, temp, end, n-1);
		hanoiTower(start, end,temp, 1);
		hanoiTower(temp, end,start, n-1);
	}
}
int main() {
	int n; cin>>n;
	hanoiTower('A','C','B', n);
}
