#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

#define MAX_LEN 128

void reverse_case(char *str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = tolower(str[i]);
		else str[i] = toupper(str[i]);
	}
}

int main(void) {
	pid_t pid;
	int fd1[2];  // child writes in this pipe.
	int fd2[2];  // parent writes in this pipe.

	pipe(fd1);
	pipe(fd2);

	pid = fork();

	if (pid < 0) {
		perror("fork");
		return 1;	
	} else if (pid == 0) { // child
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);
		char *raw_str = malloc(MAX_LEN);
		char res_str[MAX_LEN];

		size_t max_len = MAX_LEN;
		while(getline(&raw_str, &max_len, stdin) != EOF) {
			write(fd1[WRITE_END], raw_str, MAX_LEN);
			read(fd2[READ_END], res_str, MAX_LEN);

			printf("%s", res_str);
		}
		free(raw_str);
	} else {
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

		char raw_str[MAX_LEN];
		
		while(1) {
			int r = read(fd1[READ_END], raw_str, MAX_LEN);
			if (r == 0) break;
			reverse_case(raw_str);
			write(fd2[WRITE_END], raw_str, MAX_LEN);
		}
	}
		
}
