#include <bits/stdc++.h>
using namespace std;
int n=10;
int a[]={1,8,3,2,5,9,4,7,6,11};
float sum(int a[],int l,int r) {
    if(l==r) return a[l]%2!=0?a[l]:0;
    float m=(l+r)/2;
    return sum(a,l,m)+sum(a,m+1,r);
}
int main()
    int ans=sum(a,0,n-1);
    if(ans>0)
        cout<<"tong cac so le la: "<<ans<<endl;
    else cout<<"mang khong co so le"<<endl;
    return 0;
}
