#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void addChar(char* s, char c)
{
	s[strlen(s)+1]='\0';
	s[strlen(s)]=c;
}

char* encode(char* input, int c)
{
	char ** strings = malloc(c*sizeof(char*));
	for (int i=0; i<c; i++) {
		strings[i] = malloc(strlen(input)*sizeof(char));
		memset(strings[i], 0, strlen(input)*sizeof(char));
	}
	int a = 1;
	int r = 0;
	for (int i=0; i<strlen(input); i++){
		if ((input[i]>='A' && input[i]<='Z') || (input[i]>='a' && input[i]<='z')) {
			addChar(strings[r], input[i]);
			if (r==c-1) a=-1;
			if (r==0) a=1;
			r+=a;
		}
	}
	for (int i=0; i<c; i++) {
		addChar(strings[i], ' ');
	}
	for (int i=1; i<c; i++) {
		strcat(strings[0], strings[i]);
		free(strings[i]);
	}
	return strings[0];
}

char* decode(char* input, int c)
{
	int len=0;
	for (int i=0; i<strlen(input); i++){
		if (input[i]!=' ') len++;
	}
	char ** strings = malloc(c*sizeof(char*));
	for (int i=0; i<c; i++) {
		strings[i] = malloc(strlen(input)*sizeof(char));
		memset(strings[i], 0, strlen(input)*sizeof(char));
	}
	int counts[c];
	int over = len % (2*c-2);
	int base = len/(2*c-2);
	for (int i=0; i<c; i++){
		counts[i]=base;
		if (i<over) counts[i]++;
	}
	for (int i=c; i<(2*c-2); i++){
		counts[2*c-i-2]=base;
		if (i<over) counts[i]++;
	}
	char *p=input;
	int  i = 0;
	int r = 0;
	while(*p!='\0'){
		if (*p!=' ') {
			addChar(strings[r], *p);
			counts[r]--;
		}
		if (counts[r]==0){
			r++;
		}
		p++;
	}
	int a = 1;
	r = 0;
	char *out = malloc(strlen(input)*sizeof(char));
	for (int i=0; i<len; i++){
		addChar(out, strings[r][counts[r]]);
		counts[r]++;
		if (r==c-1) a=-1;
		if (r==0) a=1;
		r+=a;
	}

	for (int i=0; i<c; i++) {
		free(strings[i]);
	}

	return out;
}

int main()
{
	printf("Enter rail count:");
	int n = getInt();
	printf("Enter plain text: ");
	char *input = getPlainText();
	printf("%s\n", input);
	char *ciphertext = encode(input, n);
	printf("ciphertext: %s\n", ciphertext); 
	char *decrypted = decode(ciphertext, n);
	printf("decrypted: %s\n", decrypted); 

	free(decrypted);
	free(ciphertext);
	free(input);
}
