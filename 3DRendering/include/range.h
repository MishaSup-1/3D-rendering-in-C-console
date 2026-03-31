int min3(int a, int b, int c) {
	int num = a;
	if(b < num) num = b;
	if(c < num) num = c;
	return num;
}

int max3(int a, int b, int c) {
	int num = a;
	if(b > num) num = b;
	if(c > num) num = c;
	return num;
}
