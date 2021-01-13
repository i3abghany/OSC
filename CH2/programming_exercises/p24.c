#include <stdio.h>
#define MAX_LEN 128

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("No arguments supplied.\n");
		return 1;
	} 
	FILE *file_to_copy;
	file_to_copy = fopen(argv[1], "r");
	if (file_to_copy == NULL) {
		printf("File does not exist.\n");
		return 1;
	}
	FILE *file_to_copy_to = fopen(argv[2], "w+");
	char c;
	while((c = fgetc(file_to_copy)) != EOF) {
		int ret_val = fputc(c, file_to_copy_to);
		if (ret_val == EOF) {
			printf("ERROR WRITING A CHARACTER.\n");
			return 1;
		}
	}
	return 0;
}
