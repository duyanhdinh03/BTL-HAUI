#include<bits/stdc++.h>
#include<math.h>
using namespace std ;


float f(float x){
	return pow(x,4) - 4*x- 1 ;
}

float chiadoi(float a , float b){
	float esp = 0.001 ;
	float c = (a+b)/2;
	if(f(a)*f(b)<0){
		
		while (f(c) !=0 && abs(a-b)>esp){
			if(f(c)>0){
				a=c ;
			}else {
				b=c ;
			}
			c= (a+b)/2;
		}
		return c ;
	}
	return 0 ;
}
int main(){
	float a,b ;
	cin >> a>>b ;
	cout<< chiadoi(a,b);

	return 0 ;
}

