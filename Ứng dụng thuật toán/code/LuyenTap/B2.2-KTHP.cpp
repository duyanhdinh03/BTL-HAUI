#include <bits/stdc++.h>
using namespace std;
int n=8;
float a[]={1.1,2.2,1.2,4.3,5.6,6.4,7.3,8.2};
float C = 20;
float M = 0;
int D=0;
int greedy(){
    float now=0;
    for(int i=n-1;i>=0;--i){
        now+=a[i];
        D++;
        if(now>C)
            break;
    }
    if(now>C)
        return D;
    else return -1;

}
vector<int> boyer(string t,string p){
    int lastSeen[256];
    for(int i=0;i<256;i++)
        lastSeen[i]=-1;
    for(int i=0;i<p.size();i++)
        lastSeen[p[i]]=i;
    int s=0;
    int n=t.size();
    int m=p.size();
    vector<int> pos;
    while(s+m<=n) {
        int j=m-1;
        while(j>=0 && p[j]==t[s+j])
            j--;
        if(j<0){
            pos.push_back(s);
            s+=(s+m<n)?(m-lastSeen[t[s+m]]):1;
        }
        else s+=max(1,j-lastSeen[t[s+j]]);
    }
    return pos;
}
int main() {
    greedy();
    if(D == -1) {
		cout<<"Khong the lay!\n";
	} else {
		cout<<"Can lay it nhat "<<D<<" phan tu de dat tong lon hon C\n";
		for(int i = n - 1; i >= n-D; --i) {
			cout<<a[i]<<" ";
			M += a[i];
		}
		cout<<"\nM = "<<M<<"\n";

	}
    string P = "xin chao , ten toi la Duy";
    string Q = "chao ";
    vector<int> check=boyer(P,Q);
    if(check.size()==0) cout<<"Q khong xuat hien trong P";
    else {
        cout<<"Q xuat hien trong Q tai vi tri: "<<endl;
        for(int i:check)
            cout<<i+1<<" ";
    }
    return 0;
}
