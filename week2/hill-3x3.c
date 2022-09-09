#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int N=3;
int space=29;

void multiply(int matrix[N][N], int vec[N], int out[N]){
	for (int i=0; i<N; i++) out[i] = 0;
	for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
			out[i] += matrix[i][j] * vec[j];
	for (int i=0; i<N; i++) out[i] = out[i] % space;
}

void printMat(int matrix[N][N]){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			printf("%d\t", matrix[i][j]);
		printf("\n");
	}
}

int map(char in){
	if (in>='a' && in<='z'){
		return in-'a';
	}
	if (in>='A' && in<='Z'){
		return in-'A';
	}
	int ret;
	switch (in){
		case ' ':
			ret = 26;
			break;
		case '.':
			ret = 27;
			break;
		case '?':
		default:
			ret = 28;
			break;
	}
	return ret;
}

char revmap(int in){
	if (in<26){
		return in+'a';
	}
	int ret;
	switch (in){
		case 26:
			ret = ' ';
			break;
		case 27:
			ret = '.';
			break;
		default:
			printf("(%d)", in);
		case 28:
			ret = '?';
			break;
	}
	return ret;
}

int main(){
	int mat[3][3], invm[3][3], inv, inp[3], out[3];
	int det = 0;

	printf("Enter elements of matrix row wise:\n");
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			scanf("%d", &mat[i][j]);
	while (getchar() != '\n');
	printf("Given matrix is:\n");
	printMat(mat);

	//finding determinant
	for(int i = 0; i < 3; i++)
		det = det + ((mat[0][i] * (mat[1][(i+1)%3] * mat[2][(i+2)%3] - mat[1][(i+2)%3] * mat[2][(i+1)%3]))%space+space)%space;
	printf("determinant: %d\n", det);
	for (inv=0; inv<space; inv++) if ((inv * det)%space == 1) break;

	printf("Inverse of matrix is: \n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++)
			invm[i][j] = ((((mat[(j+1)%3][(i+1)%3] * mat[(j+2)%3][(i+2)%3]) - (mat[(j+1)%3][(i+2)%3] * mat[(j+2)%3][(i+1)%3]))*inv)%space+space)%space;
	}
	printMat(invm);

	
	int flag=1;
	char *temp = NULL;
	size_t tlen;
	ssize_t slen;
	int **matrix = (int **) invm;
	while(flag) {
		printf("Enter mode ([e] Encrypt [*] Decrypt): ");
		if ((slen=getline(&temp, &tlen, stdin))==-1){
			exit(EXIT_FAILURE);
		}
		if (slen==2 || slen==1){
			switch (temp[0]){
				case 'e':
				case 'E':
					matrix = (int **) mat;
					flag = 0;
					break;
				case 'd':
				case 'D':
					matrix = (int **) mat;
					flag = 0;
					break;
				default:
					printf("Invalid Input\n");
			}
		}
	}
	free(temp);

	char* str = NULL;
	size_t len;

	printf("Enter input string: ");
	if (getline(&str, &len, stdin)==-1){
		exit(EXIT_FAILURE);
	}
	len = (len/3 + (len%3!=0))*3+1;
	if (realloc(str, len)==NULL){
		exit(EXIT_FAILURE);
	}

	str[strlen(str)-1]='\0';

	for (int i=strlen(str), j=0; j<3; i++, j++){
		if (i%3!=0){
			str[i]=' ';
		} else {
			str[i]='\0';
			break;
		}
	}

	printf("%s\n", str);

	for (int i=0; i<strlen(str); i+=3){
		for (int j=0; j<3; j++)
			inp[j] = map(str[i+j]);
		multiply((int (*)[])matrix, inp, out);
		for (int j=0; j<3; j++)
			printf("%c",revmap(out[j]));
	}

	printf("\n");
	free(str);

	return 0;
}
