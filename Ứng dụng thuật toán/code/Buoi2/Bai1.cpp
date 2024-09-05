#include <iostream>
using namespace std;

int uocChungLonNhat(int a, int b) {
	if(a%b==0)
		return b;
	return uocChungLonNhat(b,a%b);
}

int main() {
	int a,b;
	cin>>a>>b;
	cout<<"Uoc chung lon nhat: "<<uocChungLonNhat(a,b);
}
