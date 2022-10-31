#include <stdio.h>
#include <string.h>

int main(int argc, char** argv){
	if (argc!=4){
		printf("This program takes 3 arguments exactly\n");
		return -1;
	}
	if (strcmp(argv[1], "e")==0){
	} else if (strcmp(argv[1], "d")==0){
	} else {
		printf("Invalid command: %s\n", argv[1]);
	}
	return 0;
}
