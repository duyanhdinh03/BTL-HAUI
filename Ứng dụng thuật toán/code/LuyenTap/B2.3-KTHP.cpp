#include <iostream>
#include <vector>
using namespace std;

struct GoiHang {
	int m;
	float v;
};


bool cp(GoiHang a, GoiHang b) {
	return a.v < b.v;
}


int tham_lam(GoiHang a[], int n, float c) {
	float now = 0;
	int cnt = 0;
	for(int i = 0; i < n; ++i) {
		now += a[i].v;
		cnt++;
		if(now >= c)
			break;
	}
	if(now == c) {
		return cnt;
	} else return -1;
}

void cai_tui(GoiHang a[], int n, int M, vector<vector<float>> &dp) {
	dp = vector<vector<float>> (n + 1,vector<float>(M+1, 0));

	for(int i = 1; i <= n; ++i) {
		for(int j = 1; j <= M; ++j) {
			if(a[i-1].m <= j) {
				dp[i][j] = max(dp[i-1][j], (dp[i-1][j-a[i-1].m] + a[i-1].v));
			} else dp[i][j] = dp[i-1][j];
		}
	}
}


void truy_vet(GoiHang a[], int n, int M, vector<vector<float>> &dp) {
	cout<<"Gia tri lon nhat dat duoc: "<<dp[n][M]<<"\n";
	int i = n;
	int j = M;
	while(i > 0 && j >= 0) {
		if(dp[i][j] != dp[i-1][j]) {
			cout<<a[i-1].v<<" ";
			j -= a[i -1].m;
		}
		--i;
	}
}



int main() {
	GoiHang a[] = {
	{1,6},
	{5,8},
	{4,9},
	{8,12},
	{4,14},
	{7,17},
	{9,20},
	{2,35}
	};
	int n = 8;
	int M = 5;
	float c = 23;
	int cnt = tham_lam(a, n, c);
	if(cnt == -1) cout<<"Khong the lay!\n";
	else {
		cout<<"Lay nhieu nhat "<<cnt<<" goi\n";
		for(int i = 0; i < cnt ; ++i)
			cout<<i + 1<<"\n";

	}
	vector<vector<float>> dp;
	cai_tui(a, n, M, dp);
	truy_vet(a, n, M, dp);
}
