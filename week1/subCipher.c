#include <stdio.h>

char lowerTransform(char input, char map[])
{
	if (input >= 'a' && input <= 'z'){
		char temp = input - 'a';
		temp = map[temp];
		return temp;
	}
	return input;
}

char upperTransform(char input, char map[])
{
	if (input >= 'A' && input <= 'Z'){
		char temp = input - 'A';
		temp = map[temp];
		temp = temp + 'A' - 'a';
		return temp;
	}
	return input;
}

char spaceTransform(char input)
{
	if (input == ' '){
		return '$';
	} else if (input == '$'){
		return ' ';
	}
	return input;
}

int main(int argc, char* argv[])
{
//	             "abcdefghijklmnopqrstuvwxyz"
	char map[] = "rigbxlsqenpzcuyojwfvahkmtd";
	printf("Press (e) to encrypt. Any other key to decrypt: ");
	char c;
	scanf("%c", &c);
	char* cmap;
	if (c=='e') {
		cmap = map;
	} else {
		char rev_map[] = "aaaaaaaaaaaaaaaaaaaaaaaaaa";
		for (int i=0; i<26; i++){
			rev_map[map[i]-'a']+=i;
		}
		cmap = rev_map;
	}
	while(c!='\n') scanf("%c", &c);
	scanf("%c", &c);
	while(c!='\n'){
		printf("%c",lowerTransform(upperTransform(spaceTransform(c), cmap), cmap));
		scanf("%c", &c);
	}
	printf("\n");
}
