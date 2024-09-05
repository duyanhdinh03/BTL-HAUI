#include <bits/stdc++.h>
using namespace std;
int a[5] = {100,25,10,5,1}; //mệnh giá các đồng tiền để đổi
int n;//số tiền muốn đổi
int s[100];//mảng kết quả
void input() {
    cout<<"nhap so tien muon doi: ";
    cin>>n;
}
void coinchange() {
    for(int i=0;i<5;i++) {
        s[i]=n/a[i];
        n %= a[i];
        cout<<s[i]<<" ";
    }

}
int main() {
    input();
    cout<<"S: ";
    coinchange();
    return 0;
}
