#include<iostream>
#include<string>

using namespace std;

void show(int a[]){
	string name[6] = {"Trang","Cong","Trung","Binh","Hoan","Mai"};
	for(int i=1;i<=4;i++){
		cout<<name[a[i]-1]<<" ";
	}
	cout<<endl;
}

void next(int a[], int i){
	a[i] += 1;
	i++;
	while(i<=4){
		a[i] = a[i-1] + 1;
		i++;
	}
}

void liet_ke(){
	int i, a[5] = {0};
	for(int i=1;i<=4;i++)
		a[i] = i;
	do{
		show(a);
		i=4;
		while(i>0 && a[i]==6-4+i){
			i--;
		}
		if(i>0){
			next(a,i);
		}
	}while(i>0);
}

int main(){
	liet_ke();
}
