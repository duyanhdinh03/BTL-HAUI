#include <iostream>
#include <algorithm>
using namespace std;
int coin[100];
int n;
int m;


void init() {
	cin>>n>>m;
	for(int i = 0; i < n; ++i)
		cin>>coin[i];
}



void solve() {
	sort(coin, coin + n);
	for(int i = n - 1; i >= 0; --i) {
		int numberOfCoin = m / coin[i];
		cout<<coin[i]<<": "<<numberOfCoin<<"\n";
		m %= coin[i];
	}
}



int main() {
	init();
	solve();
}
