#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 26

int** allocMat(int dim){
	int** mat=malloc(dim*sizeof(int*));
	for (int i=0; i<dim; i++)
		mat[i]=malloc(dim*sizeof(int));
	return mat;
}

void mxs(int** matrix, int s, int dim)
{
	for (int i=0; i<dim; i++)
		for (int j=0; j<dim; j++)
			matrix[i][j]*=s;
	
}

void mxv(int** matrix, int* vec, int dim, int* out)
{
	for (int i=0; i<dim; i++)
		out[i]=0;
	for (int i=0; i<dim; i++)
		for (int j=0; j<dim; j++)
			out[i]+=matrix[i][j]*vec[j];
	
}

int modN(int x)
{
	return ((x%N)+N)%N;
}

void vecModN(int* vec, int dim)
{
	for (int i=0; i<dim; i++)
		vec[i] = modN(vec[i]);
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

int det(int** matrix, int dim)
{
	if (dim==1) return matrix[0][0];
	int** temp = allocMat(dim-1);
	int cdet=0;
	int sign=1;
	for (int j=0; j<dim; j++){
		for (int i=1; i<dim; i++) {
			for (int j1=0, jj=0; j1<dim; j1++){
				if (j1==j) continue;
				temp[i-1][jj]=matrix[i][j1];
				jj++;
			}
		}
		cdet+=(sign*det(temp, dim-1)*matrix[0][j]);
		sign=-sign;
	}
	free(temp);
	return cdet;
}

int** coefMat(int** matrix, int dim)
{
	int** temp = allocMat(dim-1);
	int **out = allocMat(dim);
	int sign=1;
	for (int i=0; i<dim; i++){
		sign = 1 - 2*(i%2);
		for (int j=0; j<dim; j++){
			for (int i1=0,ii=0; i1<dim; i1++) {
				if (i1==i) continue;
				for (int j1=0, jj=0; j1<dim; j1++){
					if (j1==j) continue;
					temp[ii][jj]=matrix[i1][j1];
					jj++;
				}
				ii++;
			}
			out[i][j]=(sign*det(temp, dim-1));
			sign=-sign;
		}
	}
	free(temp);
	return out;
}

void transMat(int** matrix, int dim)
{
	for (int i=0; i<dim; i++)
		for (int j=i+1; j<dim; j++) {
			int temp = matrix[i][j];
			matrix[i][j] = matrix[j][i];
			matrix[j][i] = temp;
		}
}

void matModN(int** matrix, int dim)
{
	for (int i=0; i<dim; i++)
		for (int j=0; j<dim; j++)
			matrix[i][j] = modN(matrix[i][j]);
}

void printMat(int**  matrix, int dim)
{
	for (int i=0; i<dim; i++) {
		for (int j=0; j<dim; j++)
			printf("%d\t", matrix[i][j]);
		printf("\n");
	}
}

void inputMat(int** matrix, int dim)
{
	printf("input %d dimentional matrix\n", dim);
	for (int i=0; i<dim; i++) {
		for (int j=0; j<dim; j++)
			scanf("%d", &(matrix[i][j]));
	}
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	free(temp);
}

int getDim()
{
	int dim;
	printf("input dimension: ");
	scanf("%d", &dim);
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	free(temp);
	return dim;
}

char* getPlainText(int dim){
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	int len = strlen(temp);
	temp[len-1]='\0';
	len = strlen(temp);
	if (len%dim==0) return temp;
	len = ((len/dim)+1)*dim;
	temp = realloc(temp, (len+1)*sizeof(char));
	for (int i=strlen(temp); i<len; i++)
		temp[i]='x';
	temp[len]='\0';
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

char* encode(char* input, int **matrix, int n)
{
	char *ciphertext = malloc(strlen(input)*sizeof(char));
	ciphertext[strlen(input)]='\0';
	int *veci = malloc(n*sizeof(int));
	int *veco = malloc(n*sizeof(int));
	for (int i=0; i<strlen(input); i+=n){
		for (int j=0; j<n; j++)
			veci[j]=c2i(input[i+j]);
		mxv(matrix, veci, n, veco);
		for (int j=0; j<n; j++)
			ciphertext[i+j]=i2c(veco[j]);
	}
	free(veco);
	free(veci);
	return ciphertext;
}

int main()
{
	int n = getDim();
	int **matrix=allocMat(n);
	int d, invert;
	
	while (1) {
		inputMat(matrix, n);
		printMat(matrix, n);
		d = det(matrix, n);
		invert = inv(modN(d));
		if (invert!=-1) break ;
	}

	int **decode = coefMat(matrix, n);
	transMat(decode, n);
	mxs(decode, invert, n);
	matModN(decode, n);
	printMat(decode, n);

	printf("Enter plain text: ");
	char *input = getPlainText(n);
	printf("%s\n", input);
	char *ciphertext = encode(input, matrix, n);
	printf("%s\n", ciphertext); 
	char *decrypted = encode(ciphertext, decode, n);
	printf("%s\n", decrypted); 

	free(decrypted);
	free(ciphertext);
	free(input);

	free(decode);
	free(matrix);
}
