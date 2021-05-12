int func(int a, int b, int c, int d, int e){
	return 1;
}

// int func2(double a, double b, double c){
// 	return 1;
// }


int main(int argc, int* argv[]){
	int i =0;
	for(; i < 500000; i++){
		func(1, 1, 1, 1, 1);
	}
	return 0;
}
