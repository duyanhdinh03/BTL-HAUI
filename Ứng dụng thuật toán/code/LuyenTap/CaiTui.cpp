#include <bits/stdc++.h>
using namespace std;
int n;
int k;
int w[100];
int v[100];
int res[100][100];
int t;
int take[100];

void init() {
    cin>>n>>k;
    for(int i = 0; i < n; ++i)
        cin>>w[i+1]>>v[i+1];
    for(int i = 0; i <= n ; ++i)
        res[i][0] = 0;
}


int solve() {
    for(int i = 1; i <= n; ++i) {
        for(int j = 1; j <= k; ++j) {
            if(w[i]<=j)
                res[i][j] = max(res[i-1][j],res[i-1][j-w[i]] + v[i]);
            else res[i][j] = res[i-1][j];
        }
    }
    return res[n][k];
}

void show() {
    for(int i = 0; i <=k ; ++i) {
        for(int j = 0; j <= n; ++j)
            cout<<setw(5)<<res[j][i];
        cout<<"\n";
    }
}


void trace() {
    int W = k;
    int N = n;
    for(int i = N; i > 0; --i) {
        if(res[i][W] != res[i-1][W]) {
            take[t++] = i;
            W -= w[i];
        }
    }
    cout<<"lay goi: ";
    for(int i = t-1; i >= 0; --i)
        cout<<take[i]<<" ";
}

int main() {
    init();
    int res = solve();
    trace();
}

