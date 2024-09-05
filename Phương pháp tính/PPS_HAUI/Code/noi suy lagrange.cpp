#include <bits/stdc++.h>

using namespace std ;


double lagrangeInterpolation(double x[], double y[], int n, double target) {
	double result = 0.0;

	for (int i = 0; i < n; ++i) {
		double term = y[i];

		for (int j = 0; j < n; ++j) {
			if (j != i) {
				term *= (target - x[j]) / (x[i] - x[j]);
			}
		}

		result += term;
	}

	return result;
}

int main() {
	int n =4 ;
	double x[] = {-1.0, 0.0, 1.0, 2.0}; // Giá trị x
	double y[] = {0.5, 1.0, 2.0, 4.0}; // Giá trị y

	double x_value_1 = -0.5; // Giá trị x để nội suy
	double x_value_2 = 0.7; // Giá trị x để nội suy


	double interpolatedValue_1 = lagrangeInterpolation(x, y, n, x_value_1);
	double interpolatedValue_2 = lagrangeInterpolation(x, y, n, x_value_2);

	cout << "Da thuc noi suy tai x = " << x_value_1 << " la: " << interpolatedValue_1 << endl;
	cout << "Da thuc noi suy tai x = " << x_value_2 << " la: " << interpolatedValue_2 << endl;


	return 0;
}