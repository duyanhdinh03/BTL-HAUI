#include <iostream>
#include <algorithm>
using namespace std;


string d[] = {
"hi",
"hi, my name is Giang",
"hi, nice to meet you",
"Hello, this is my child",
"Goodbye, see you again",
"Thank you very much"
};
int n = 6;
int k = 22;

bool cp(string a, string b) {
	return a.size() < b.size();
}


void greedy() {
	sort(d, d + n, cp);
	int now = 0;
	int res[100];
	int count = 0;
	for(int i = n - 1; i >= 0; --i) {
		if(now + d[i].size() <= k) {
			now += d[i].size();
			res[count++] = i;
		}
	}
	if(now == k) {
		cout<<"Can lay it nhat "<<count<<" xau\n";
		for(int i = 0; i < count; ++i) {
			cout<<d[res[i]]<<"\n";
		}
	} else cout<<"Khong the dat do dai dung bang "<<"k\n";
}


int boyer(string t, string p) {
	int lastSeen[256];
	for(int i = 0; i < 256; ++i)
		lastSeen[i] = -1;
	for(int i = 0; i < p.size(); ++i)
		lastSeen[p[i]] = i;


	int n = t.size();
	int m = p.size();
	int s = 0;
	int cnt = 0;
	while(s + m<= n) {
		int j = m - 1;
		while( j >= 0 && p[j] == t[s + j])
			j--;
		if(j < 0 ) {
			cnt ++;
			s += (s + m < n) ? (m - lastSeen[t[s + m]]) : 1;
		} else s+= max(1, j - lastSeen[t[s+j]]);
	}
	return cnt;
}


void howManyChild(string d[], int n) {
	cout<<"Cac xau xuat hien child: \n";
	for(int i = 0; i < n; ++i) {
		int cnt = boyer(d[i], "child");
		if(cnt > 0)
			cout<<d[i]<<" | "<<cnt<<"\n";
	}
}


bool isSubstring(string t, string p) {
	string s = p + "$" + t;
	int n = s.size();
	vector<int> z(n, 0);
	int l = 0;
	int r = 0;
	for(int i = 0; i < n; ++i) {
		if( i < r)
			z[i] = min(r-i + 1, z[i-l]);
		while(i + z[i] < n && s[z[i]] == s[i + z[i]])
			z[i]++;
		if(i + z[i] - 1 > r) {
			r = i + z[i] -1;
			l = i;
		}
		if(z[i] == p.size() && i > p.size())
			return true;
	}
	return false;
}


void d0Substring(string d[], int n) {
	cout<<"Cac xau chua xau "<<d[0]<<":\n";
	for(int i = 1; i < n; ++i) {
		if(isSubstring(d[i], d[0]) ) {
			cout<<d[i]<<"\n";
		}
	}
}


int main() {
	greedy();
	howManyChild(d, n);
	d0Substring(d, n);
}
