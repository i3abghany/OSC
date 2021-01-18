#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

static const char *const prompt = "osh> ";
#define MAX_LINE_LEN 256
#define MAX_ARG_LEN 32
#define MAX_ARGS 32

#define HISTORY_CAP 128

#define READ_END 0
#define WRITE_END 1

static char *history[MAX_LINE_LEN]; 
static unsigned int history_top;

void parse_args(char *line, char **args);
void print_args(char **args);
unsigned int num_args(char **args);
void append_history(char *line);

void strip_line(char **line);

void handle_history(unsigned int cap);

int main(void)
{
	int run = 1;
	char *args[MAX_ARG_LEN];

	for (int i = 0; i < MAX_ARGS; i++)
		args[i] = malloc(sizeof(char *));

	for (int i = 0; i < HISTORY_CAP; i++)
		history[i] = malloc(sizeof(char *));

	char line_orig[MAX_LINE_LEN];
	char *orig_ptr = &line_orig[0];

	char line_storage[MAX_LINE_LEN];
	char *line = &line_storage[0];

	history_top = 0;

	while (run) {
		printf("%s", prompt);
		fflush(stdout);
		fgets(line_orig, MAX_LINE_LEN, stdin);
		strip_line(&orig_ptr);
		if (strcmp(line_orig, "\n") == 0) continue;

		strcpy(line_storage, line_orig);
		parse_args(line, args);
		
		unsigned int num = num_args(args);

		args[num] = malloc(sizeof(char) * MAX_ARG_LEN);

		if (strcmp(args[0], "history") == 0) {
			unsigned int cap = 0;
			if(num >= 2) cap = (unsigned int) strtol(args[1], NULL, 10); 
			handle_history(cap);
			append_history(line_orig);		
			continue;
		}

		
		pid_t pid = fork();
		
		if (pid < 0) {
			perror("fork");
			continue;
		} else if (pid == 0) {
			free(args[num]);
			args[num] = NULL;
			if (num >= 3 && 
				((strcmp(args[num - 2], ">") == 0) ||
				 (strcmp(args[num - 2], "2>") == 0))) { // output redirection.
				int fd = open(args[num - 1], O_CREAT | O_RDWR, 0666);
				if (fd == -1) {
					perror(args[num - 1]);
					return 1;
				}
				if (strcmp(args[num - 2], ">") == 0) {
					dup2(fd, STDOUT_FILENO);
				} else {
					dup2(fd, STDERR_FILENO);
				}
				free(args[num - 2]); free(args[num - 1]);
				args[num - 2] = NULL;
			} else if (num >= 3 && (strcmp(args[num - 2], "<") == 0)) {
				int fd = open(args[num - 1], O_RDONLY, 0666);
				if (fd == -1) {
					perror(args[num - 1]);
					return 1;
				}
				dup2(fd, STDIN_FILENO);
				free(args[num - 2]); free(args[num - 1]);
				args[num - 2] = NULL;
			}
			execvp(args[0], args);
			perror(args[0]);
		} else {
			if (strcmp(args[num - 1], "&") != 0) {
				int stat;
				wait(&stat);
				// If control reached this point, exec never returned.
				// Thus, the command can be appended to history.
				append_history(line_orig);
			}
		}
	}

	for (int i = 0; i < MAX_ARGS; i++)
		free(args[i]);

	for (int i = 0; i < HISTORY_CAP; i++)
		free(history[i]);
	return 0;
}

void parse_args(char *line, char **args) 
{
	char *tok = NULL;
	int i = 0;
	while ((tok = strsep(&line, " "))) {
		strcpy(args[i++], tok);
	}
	args[i - 1][strlen(args[i - 1]) - 1] = '\0';

	free(args[i]);
	args[i] = NULL;
}

unsigned int num_args(char **args) 
{
	unsigned int ret = 0;
	while (args[ret++] != NULL);

	return ret - 1;
}

void print_args(char **args)
{
	printf("%s", args[0]);
	for (int i = 1; args[i] != NULL; i++) {
		printf(" %s", args[i]);
	}
	fflush(stdout);
}

void strip_line(char **line) 
{
	char c = (*line)[0];
	while (isspace(c))
		(*line)++, c = (*line)[0];
}

void append_history(char *line) 
{
	// TODO: circularly append extra commands.
	if (history_top == HISTORY_CAP - 1)
		return;

	strcpy(history[history_top++], line);
}

void handle_history(unsigned int cap) 
{
	if (cap >= history_top) cap = 0;

	unsigned int i;
	if (cap == 0) {
		i = 0;
	} else {
		i = history_top - cap;
	}

	for (; i < history_top; i++) {
		printf("%d: %s", i, history[i]);
	}
}
