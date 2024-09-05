#include<bits/stdc++.h>


using namespace std;

const int MAX_DEGREE = 20;

int n;
float a[MAX_DEGREE + 1];
float c;

void nhap_ham() {
    cout << "Nhap bac cua da thuc: ";
    cin >> n;
    cout << "Nhap cac he so: " << endl;
    for (int i = 0; i <= n; i++) {
        cout << "a[" << i << "] = ";
        cin >> a[i];
    }
}

float tinh_ham(float x) {
    float p = a[0];
    for (int i = 1; i <= n; i++) {
        p = p * x + a[i];
    }
    return p;
}

int main() {
    cout << "Nhap gia tri can tinh: c = ";
    cin >> c;

    cout << "Nhap ham pn: " << endl;
    nhap_ham();
    float A = tinh_ham(c);

    cout << "Nhap ham pm: " << endl;
    nhap_ham();
    float B = tinh_ham(c);

    cout << "\nKet qua pn(c) + pm(c) = " << A + B << endl;

    return 0;
}

