#include <iostream>
#define n 7
#define step 8
using namespace std;

int xStep[] = {1,1,-1,-1,2,2,-2,-2};
int yStep[] = {2,-2,2,-2,1,-1,1,-1};

int chessboard[n][n];
bool mark[n][n];


void init() {
	for(int i = 0 ;i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			mark[i][j] = 0;
		}
	}
	mark[0][0] = true;
	chessboard[0][0] = 1;
}

void show() {
	for(int i = 0 ;i < n; ++i) {
		for(int j = 0; j < n; ++j) {
			cout<<chessboard[i][j] <<" ";
		}
		cout<<"\n";
	}
	cout<<"\n\n\n";
}


bool isSafe(int x, int y) {
	if(x<0 || y<0 || x>=n || y>= n)
		return false;
	if(mark[x][y] == true)
		return false;
	return true;
	
}

void knight(int x, int y, int k = 2) {
	//duyet qua cac buoc di ke tiep 
	// neu chua di va hop le
		// danh dau da di
		// danh dau so thu tu
		// neu k = n * n
			// show()
		//else
			// knigt(k + 1, buoc di tiep theo)
		// bo dau nuoc di
	for(int i = 0; i < step; ++i) {
		int nextx = x + xStep[i];
		int nexty = y + yStep[i];
		if(isSafe(nextx, nexty)) {
			mark[nextx][nexty] = true;
			chessboard[nextx][nexty] = k;
			if(k == n * n) {
				show();
			} 
			else {
				knight(nextx, nexty, k + 1);
			}
			mark[nextx][nexty] = false;
	
		} 
	}
}

using namespace std;
int main() {
	init();
	knight(0,0);
}
