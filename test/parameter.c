int f(int a){
	int c;
	c = a + 3;
	return c; 
}

float f2(float b, float c){
	float d;
	d = b + c;
	return d;
}

int f3(int b, int c){
	int d;
	d = b + c;
	return d;
}

int f4(int b[4][4]){
	int c[4][4];
	c[0][0] = 2;
	b[0][0] = 1;
	write(b[0][0]);
	write("\n");
	write(c[0][0]);
	write("\n");
	return 0;
}

int MAIN(){
	int b;
	float c;
	float e;
	int g[4][4];
	c = 1.4;
	e = 1.5;
	b = 1;
	b = f(3);
	f4(g);
	write(b);
	write("\n");
	c = f2(c, e);
	b = f3(b, b);
	write(b);
	write("\n");
	write(c);
	write("\n");
	return 0;
}
