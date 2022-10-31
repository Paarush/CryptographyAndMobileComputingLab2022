#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b, int *x, int *y){
	if (a==0){
		if (x!=NULL) *x=0;
		if (y!=NULL) *y=1;
		return b;
	}
	int x1, y1;
	int g = gcd(b%a, a, &x1, &y1);
	if (x!=NULL) *x = y1 - (b/a) * x1;
	if (y!=NULL) *y = x1;
	return g;
}

unsigned int pown(int x, int y, int n){
	unsigned int a=1;
	for (int i=0; i<y; i++){
		a=(a*x)%n;
	}
	return a;
}

int main(int argc, char **argv){
	if (argc!=5){
		printf("Program takes exactly 4 arguments\n");
		printf("%s p q e x\n", argv[0]);
		return -1;
	}
	int p, q, e;
	unsigned char x;
	p = atoi(argv[1]);
	q = atoi(argv[2]);
	e = atoi(argv[3]);
	x = atoi(argv[4]);
	int n=p*q;
	int l = (p-1)*(q-1)/gcd(p-1,q-1, NULL, NULL);
	int td;
	if (gcd(e, l, &td, NULL)!=1){
		printf("Invalid e value: e not coprime to l (%d, %d)\n", e, l);
		return -1;
	}
	unsigned int d = (l+td%l)%l;
	unsigned char c = pown(x, e, n);
	unsigned char m = pown(c, d, n);
	printf("Encrypted value=%d\nDecrypted value=%d\n", c, m);
	return 0;
}
