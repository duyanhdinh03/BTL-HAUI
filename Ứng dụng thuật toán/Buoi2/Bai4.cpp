#include<bits/stdc++.h>

using namespace std;

int dem=0;
struct Call{
    int n;
    char a;
    char b;
    char c;
};

struct Stack{
	int top;
	Call *e;
};

void init(Stack &s){
    s.top = -1;
    s.e = NULL;
}

bool empty(Stack s){   
	return (s.top == -1);
}

void push(Stack &s, Call call){
    if(empty(s)){
    	s.e = new Call[1];
	} else {
		s.e = (Call*) realloc(s.e, (s.top + 2) * sizeof(Call));
	}
	s.top++;
	s.e[s.top] = call;
}

void pop(Stack &s, Call &call){
	if(empty(s))
		return;
	call = s.e[s.top];
	if(s.top == 0){
		delete []s.e;
		init(s);
	} else {
		s.e = (Call*) realloc(s.e, s.top * sizeof(Call));
		s.top--;
	}
}

void chuyen(int n, char a, char b, char c){
	Call call = {n,a,b,c}, call_in, call_out;
	Stack s;
	init(s);
	call_in = call;
	push(s, call_in);
	while(!empty(s)){
		pop(s, call_out);
		if(call_out.n==1){
			cout<<dem+1<<": "<<call_out.a<<" sang "<<call_out.c<<endl;
			dem++;
		}	
		else{
			//(n-1, b, a, c)
			call_in = {call_out.n-1, call_out.b, call_out.a, call_out.c};
			push(s, call_in);
			
			//(1, a, b, c)
			call_in = call_out;
			call_in.n = 1;
			push(s, call_in);
			
			//(n-1, a, c, b)
			call_in = {call_out.n-1, call_out.a, call_out.c, call_out.b};
			push(s, call_in);
		}
	}
}

int main(){
	int n;
	cin>>n;
	chuyen(n,'A','B','C');
	cout<<"So lan chuyen: "<<dem;
}
