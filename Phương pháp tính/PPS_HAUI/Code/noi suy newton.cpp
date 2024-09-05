#include <bits/stdc++.h>

double dividedDifference(double x[], double y[], int n, int k) {
    if (k == 0) {
        return y[0];
    } else {
        return (dividedDifference(x, y, n, k - 1) - dividedDifference(x, y, n - 1, k - 1)) / (x[k] - x[0]);
    }
}

double newtonInterpolation(double x[], double y[], int n, double target) {
    double result = 0.0;

    for (int i = 0; i < n; ++i) {
        double term = dividedDifference(x, y, n, i);

        for (int j = 0; j < i; ++j) {
            term *= (target - x[j]);
        }

        result += term;
    }

    return result;
}

int main() {
    int n;
    ucout << "Nhap n: ";
    cin >> n;

    double x[n], y[n];
    cout << "Nhap cac gia tr? x:\n";
    for (int i = 0; i < n; ++i) {
        cout << "Nhap x[" << i << "]: ";
        cin >> x[i];
        cout << "Nhap y[" << i << "]: ";
        cin >> y[i];
    }

    double x_value;
    cout << "Nhap gia tr? x de noi suy: ";
    cin >> x_value;

    double interpolatedValue = newtonInterpolation(x, y, n, x_value);

    cout << "da th?c noi suy tai x = " << x_value << " la: " << interpolatedValue << endl;

    return 0;
}
