#include <iostream>
using namespace std;

int sumOdd(int arr[], int n, int i = 0) {
	if(i==n) {
		return 0;
	}
	return (arr[i] %2 == 0 ? 0: arr[i] )  + sumOdd(arr, n, i + 1);
}


int main() {
	int arr[] ={1,2,3,4,5};
	cout<<sumOdd(arr, 5);
}
