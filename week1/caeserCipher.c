#include <stdio.h>

char lowerTransform(char input, int key)
{
	key = (26 + (key%26))%26;
	if (input >= 'a' && input <= 'z'){
		char temp = input - 'a';
		temp = (temp + key)%26;
		temp = temp + 'a';
		return temp;
	}
	return input;
}

char upperTransform(char input, int key)
{
	key = (26 + (key%26))%26;
	if (input >= 'A' && input <= 'Z'){
		char temp = input - 'A';
		temp = (temp + key)%26;
		temp = temp + 'A';
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
	printf("Enter key: ");
	int key;
	scanf("%d", &key);
	char c;
	scanf("%c", &c);
	scanf("%c", &c);
	while(c!='\n'){
		printf("%c",lowerTransform(upperTransform(spaceTransform(c), key),key));
		scanf("%c", &c);
	}
	printf("\n");
}

