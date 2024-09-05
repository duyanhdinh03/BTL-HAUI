#include <bits/stdc++.h>
using namespace std;
int n=10;
float a[]={1,-8,3,2,5,-9,4,7,6,-11};
float sum(float a[], int l,int r){
    if(l==r) return a[l]>0?a[l]:0;
    int m=(l+r)/2;
    return sum(a,l,m)+sum(a,m+1,r);
}
int main(){
    int ans=sum(a,0,n-1);
    if(ans>0)
        cout<<"tong cac so duong la: "<<ans<<endl;
    else cout<<"mang ko co so duong"<<endl;
    return 0;
}
