#include <iostream>
#include <algorithm>
using namespace std;
struct Package {
	string id;
	int s;
	int m;
};



Package package[] = {
{"P001", 10, 100},
{"P002", 60, 200},
{"P003", 20, 500},
{"P004", 50, 400},
{"P005", 70, 300},
{"P006", 30, 150}
};
int n = 6;
int w = 500;

bool cp(Package a, Package b) {
	return b.m > a.m;
}

void solve() {
	sort(package, package + n, cp);
	int now = 0;
	for(int i = 0; i < n; ++i ) {
		if(now + package[i].m > w)
			break;
		now += package[i].m;
		cout<<package[i].id<<" ";
	}
}




int main() {
	solve();
}
