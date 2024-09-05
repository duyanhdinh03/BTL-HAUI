#include <bits/stdc++.h>
using namespace std;
struct HangHoa {
    string name;
    int weight;
    int price;
};
HangHoa d[]= {
        {"H001",4,500000},
        {"H002",3,600000},
        {"H003",5,700000},
        {"H004",6,800000},
        {"H005",7,900000},
        {"H006",8,1000000},
};
int n=6;
int index[6];
int p=700000;
bool isChose[100];
void init() {
    for (int i = 0; i < n; ++i) {
        index[i]=i;
        isChose[i]= false;
    }
}
void show() {
    for (int i = 0; i < n; ++i) {
        cout<<d[index[i]].name<<":"<<i<<" ";
    }
    cout<<endl;
}
void BackTrack(int pos=0) {
    for (int i = 0; i < n; ++i) {
        if (isChose[i]==false) {
            index[pos]=i;
            isChose[i] = true;
            if (pos==n-1)
                show();
            else BackTrack(pos+1);
            isChose[i]=false;
        }
    }
}
int Total(int l, int r) {
    if(l==r)
        return d[l].price;
    int m = (l+r)/2;
    return Total(l,m) + Total(m+1,r);
}
void Pricelowerthan700000(int l,int r) {
    if (l==r){
        if(d[l].price<p) cout<<d[l].name<<" ";
    } else {
        int m=(l+r)/2;
        Pricelowerthan700000(l,m);
        Pricelowerthan700000(m+1,r);
    }
}
int main() {
    cout<<"ToTal Price: "<<Total(0,5)<<endl;
    cout<<"Thing have price lower than 700000:"<<endl;
    Pricelowerthan700000(0,5);
    cout<<endl;
    cout<<"BackTrack:"<<endl;
    init();
    BackTrack();
}
