#include <bits/stdc++.h>
using namespace std;
int n=8;
float a[]={1.1,2.2,1.2,4.3,5.6,6.4,7.3,8.2};
float C = 4.6;
int D=0;
float M=0;
int greedy(){
    for(int i=0;i<n;i++){
        if(C<a[i])
            break;
        C-=a[i];
        D++;
    }
    return D;
}
bool boyer(string t, string p){
    int lastSeen[256];
    for(int i=0;i<256;i++)
        lastSeen[i]=-1;
    for(int i=0;i<p.size();i++)
        lastSeen[p[i]]=i;
    int n=t.size();
    int m=p.size();
    int s=0;
    while(s+m<=n){
        int j=m-1;
        while(j>=0 && p[j]==t[s+j])
            j--;
        if(j<0){
            return true;
        } else s+= max(1,j-lastSeen[t[s+j]]);
    }
    return false;
}
int main() {
    greedy();
    if(D>0){
        cout<<"So luong phan tu nhieu nhat co the lay trong day a la:"<<D<<endl;
        cout<<"Do la: ";
        for(int i=0;i<D;i++){
            cout<<a[i]<<" ";
            M+=a[i];
        }
        cout<<endl;
        cout<<"M= "<<M<<endl;
    }
    else cout<<"Khong co gia tri thoa man"<<endl;
    string P = "xin chao , ten toi la Duy";
    string Q = "xin chao ";
    bool isSubstring = boyer(P,Q);
    if(isSubstring) cout<<"chuoi Q la chuoi con cua chuoi P"<<endl;
    else cout<<"chuoi Q khong phai chuoi con cua chuoi P"<<endl;

    return 0;
}
