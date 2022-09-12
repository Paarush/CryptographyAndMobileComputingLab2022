#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 26

int modN(int x)
{
	return ((x%N)+N)%N;
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

char* encode(char* input, int b)
{
	char *ciphertext = malloc(strlen(input)*sizeof(char));
	ciphertext[strlen(input)]='\0';
	for (int i=0; i<strlen(input); i++){
		ciphertext[i]=i2c(modN(c2i(input[i])+b));
	}
	return ciphertext;
}

char* decode(char* input, int b)
{
	char *ciphertext = malloc(strlen(input)*sizeof(char));
	ciphertext[strlen(input)]='\0';
	for (int i=0; i<strlen(input); i++){
		ciphertext[i]=i2c(modN(c2i(input[i])-b));
	}
	return ciphertext;
}

int main()
{
	printf("Enter b value:");
	int b = modN(getInt());
	printf("Enter plain text: ");
	char *input = getPlainText();
	printf("%s\n", input);
	char *ciphertext = encode(input, b);
	printf("ciphertext: %s\n", ciphertext); 
	for (int i=0; i<26; i++){
		char *decrypted = decode(ciphertext, i);
		if (strcmp(decrypted, input)==0){
			printf("encryption key: %d\n", i);
			printf("decrypted text: %s\n", decrypted);
			i=27;
		}
		free(decrypted);
	}
	free(ciphertext);
	free(input);
}
