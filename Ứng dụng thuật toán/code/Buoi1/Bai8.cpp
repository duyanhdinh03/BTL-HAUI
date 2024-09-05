#include <iostream>
#include <cstdlib>
#define n 4
#define m 4
#define start 1
#define end 5
using namespace std;

int map[n][m];
int visited[n][m];
void show() {
	for(int i = 0 ; i < n ; ++i) {
		for(int j = 0 ; j < m ; ++j) {
			cout<<map[i][j]<<" ";
		}
		cout<<"\n";
	}
}

void init() {
	for(int i = 0 ; i < n ; ++i) {
		for(int j = 0 ; j < m ; ++j) {
			map[i][j] = start + rand()%(end - start + 1);
			visited[n][m] = 0;
		}
	}
}


void mark(int i, int j) {
	if(i>=n || j >= m || i<0 || j<0)
		return;
	if(visited[i][j])
		return;
	visited[i][j] = true;
	if(i+1 < n) {
		if(map[i][j]==map[i+1][j])
			mark(i+1,j);
			
	}
	if(i-1 >= 0) {
		if(map[i][j]==map[i-1][j])
			mark(i-1,j);
			
	}
	if(j+1 < m) {
		if(map[i][j]==map[i][j+1])
			mark(i,j+1);
			
	}
	if(j-1 >=0) {
		if(map[i][j]==map[i][j-1])
			mark(i,j-1);
			
	}
		
}


int count() {
	int count = 0;
	for(int i = 0 ; i < n ; ++i) {
		for(int j = 0 ; j < m ; ++j) {
			if(!visited[i][j]) {
				mark(i,j);
				count++;
			}
		}
	}
	return count;
}

int main() {
	init();
	show();
	cout<<"So mien lien thong: "<<count();
}
