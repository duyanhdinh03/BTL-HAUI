#include<bits/stdc++.h>
using namespace std;
int n=10;
int a[]={1,8,3,2,5,9,4,7,6,12};
float sum(int l, int r, int a[]) {
    if(l==r) return a[l]>0?a[l]:0;
    int m=(l+r)/2;
    return sum(l,m,a)+sum(m+1,r,a);
}
int main() {
    cout<<"total: "<<sum(0,n-1,a)<<endl;
    return 0;
}

