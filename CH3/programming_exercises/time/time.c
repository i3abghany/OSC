#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


char **construct_argv(char **argv, int argc);
void free_argv(char **argv, int argc);

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Supply an executable as a command line argument.\n");	
		return 1;
	}
	const char *prog = argv[1];
 
	char **new_argv = construct_argv(argv, argc);

	pid_t pid;

	pid = fork();
	if (pid < 0) {
		free_argv(new_argv, argc);
		perror("fork");	
		return 1;
	}
	else if (pid == 0) {
		execvp(prog, new_argv);
		printf("no exec");
	} else {
		wait(NULL);
	}
}

char **construct_argv(char **argv, int argc)
{
	char **new_argv = malloc((argc) * sizeof(char *));
	new_argv[argc - 1] = NULL;
	for (int i = 1; i < argc; i++) {
		new_argv[i - 1] = argv[i];
	}
	return new_argv;
}

void free_argv(char **argv, int argc)
{
	for (int i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);
}
