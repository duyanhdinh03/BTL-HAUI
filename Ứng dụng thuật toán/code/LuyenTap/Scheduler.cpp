#include <iostream>
#include <algorithm>
using namespace std;
struct Task {
	int start, end;
	string id;
};


Task tasks[] = {
{1,4,"T001"},
{3,5,"T002"},
{4,6,"T003"},
{3,9,"T004"},
{1,5,"T005"},
{5,7,"T006"}
};
int n = 6;


bool cp(Task a, Task b) {
	return b.end > a.end;
}


void solve() {
	sort(tasks, tasks + n, cp);
	int lastEnd = 0;
	for(int i = 0; i < n; ++i) {
		if(tasks[i].start >= lastEnd) {
			cout<<tasks[i].id<<" ";
			lastEnd = tasks[i].end;
		}
	}
}






int main() {
	solve();
}
