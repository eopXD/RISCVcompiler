int f(int a[4][4]){
	int b[4][4];
	a[0][0] = 1;
	b[0][0] = 2;
	write(a[0][0]);
	write("\n");
	write(b[0][0]);
	write("\n");
	return 0;
}

int MAIN(){
	int a[4][4];
	f(a);
	return 0;
}
