
#include "iostream"
using namespace std;
string p;
string t;


bool isSubstring(string p, string t) {
    int n = t.size();
    int m = p.size();
    for(int i = 0; i + m <= n ; ++i) {
        int j = 0;
        while(t[i + j] == p[j] && j < m)
            ++j;
        if(j != m)
            continue;
        return true;
    }
    return false;
}


int main() {
    if(isSubstring("xin", "chxinao"))
        cout<<"Co";
    else cout<<"Khong";
}
