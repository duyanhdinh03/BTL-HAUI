#include <iostream>
using namespace std;
int uocChungLonNhat(int a, int b) {
	while(a%b!=0) {
		int temp = a;
		a = b;
		b = temp % b;
	}
	return b;
}

int main() {
	int a, b;
	cin>>a>>b;
	cout<<"Uoc chung lon nhat: "<<uocChungLonNhat(a,b);
}
