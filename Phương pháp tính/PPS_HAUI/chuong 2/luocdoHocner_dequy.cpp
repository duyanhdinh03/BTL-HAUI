#include<bits/stdc++.h>

using namespace std;

const int MAX_DEGREE = 20;

float a[MAX_DEGREE + 1];
float c;

float dequy(int i) {
    if (i == 0) return a[0];
    else return dequy(i - 1) * c + a[i];
}

int main() {
    int n;
    cout << "Nhap bac da thuc: ";
    cin >> n;
    cout << "Nhap cac he so: ";
    for (int i = 0; i <= n; i++) {
        cout << "a[" << i << "] = ";
        cin >> a[i];
    }

    cout << "Nhap c = ";
    cin >> c;

    cout << "Ket qua: " << dequy(n) << endl;
    return 0;
}

