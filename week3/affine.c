#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 26

int modN(int x)
{
	return ((x%N)+N)%N;
}

int extendedGCD(int a, int b, int *x, int* y)
{
	if (a==0){
		*x=0;
		*y=1;
		return b;
	}
	int x1=0, y1=0;
	int gcd = extendedGCD(b%a, a, &x1, &y1);
	*x = y1 - (b/a) * x1;
	*y = x1;
	return gcd;
}

int inv(int v)
{
	int x=0,y=0,n;
	n=extendedGCD(v, N, &x, &y);
	if (n==1)
		return modN(x);
	return -1;
}

int getInt()
{
	int dim;
	scanf("%d", &dim);
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	free(temp);
	return dim;
}

char* getPlainText(){
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	int len = strlen(temp);
	temp[len-1]='\0';
	return temp;
}

int c2i(char c){
	if (c>='a' && c<='z') return c-'a';
	if (c>='A' && c<='Z') return c-'A';
	return 0;
}

char i2c(int n){
	return 'a'+modN(n);
}

char* encode(char* input, int a, int b)
{
	char *ciphertext = malloc(strlen(input)*sizeof(char));
	ciphertext[strlen(input)]='\0';
	for (int i=0; i<strlen(input); i++){
		ciphertext[i]=i2c(modN(a*c2i(input[i])+b));
	}
	return ciphertext;
}

char* decode(char* input, int a, int b)
{
	int a1=inv(a);
	char *ciphertext = malloc(strlen(input)*sizeof(char));
	ciphertext[strlen(input)]='\0';
	for (int i=0; i<strlen(input); i++){
		ciphertext[i]=i2c(modN(a1*(c2i(input[i])-b)));
	}
	return ciphertext;
}

int main()
{
	int a;
	while(1){
		printf("Enter a value:");
		a = modN(getInt());
		if (inv(a)!=-1) break;
	}
	printf("Enter b value:");
	int b = modN(getInt());
	printf("Enter plain text: ");
	char *input = getPlainText();
	printf("%s\n", input);
	char *ciphertext = encode(input, a, b);
	printf("ciphertext: %s\n", ciphertext); 
	char *decrypted = decode(ciphertext, a, b);
	printf("decrypted: %s\n", decrypted); 

	free(decrypted);
	free(ciphertext);
	free(input);
}
