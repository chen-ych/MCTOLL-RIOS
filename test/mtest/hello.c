//extern int a;
// extern int func2();
/*
int func(int a, int b, int c){
	return a+b;
}*/

// int func2(double a, double b, double c){
// 	return 1;
// }

#include<stdio.h>

int fn( int x ) {
	return x;
}

int main(int argc, int* argv[]){
	int i = fn(5);
	return i;
}
