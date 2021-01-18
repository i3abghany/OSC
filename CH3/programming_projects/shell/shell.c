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

static unsigned int no_of_args;

void alloc_args(char *args[MAX_ARG_LEN]);
void init_args(char **args, char *args_1[MAX_ARG_LEN], char *args_2[MAX_ARG_LEN]);
unsigned int num_args(void);

void strip_line(char **line);
unsigned int parse_args(char *line, char **args);
void print_args(char **args);

void append_history(char *line);


void handle_history(unsigned int cap);

int pipe_idx(char **args);
int correct_pipe(char **args);
void handle_pipe(char **args);

int main(void)
{
	int run = 1;
	char *args[MAX_ARG_LEN];
	alloc_args(args);

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
		no_of_args = parse_args(line, args);
		unsigned int num = num_args();

		if (correct_pipe(args)) {
			handle_pipe(args);
			append_history(line_orig);		
			fflush(stdout);
			continue;
		}

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
			printf("hello\n");
			perror(args[0]);
			continue;
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

unsigned int parse_args(char *line, char **args) 
{
	char *tok = NULL;
	unsigned int i = 0;
	while ((tok = strsep(&line, " "))) {
		strcpy(args[i++], tok);
	}
	args[i - 1][strlen(args[i - 1]) - 1] = '\0';

	return (unsigned int) i;
}

unsigned int num_args() 
{
	return no_of_args;
}

void print_args(char **args)
{
	printf("%s", args[0]);
	unsigned int num = num_args();
	for (unsigned int i = 1; i < num; i++) {
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

void init_args(char **args, char *args_1[MAX_ARG_LEN], char *args_2[MAX_ARG_LEN])
{
	unsigned int idx = (unsigned int) pipe_idx(args);
	unsigned int num = num_args();

	alloc_args(args_1);
	alloc_args(args_2);

	for (unsigned int i = 0; i < idx; i++) {
		strcpy(args_1[i], args[i]);
	}
	free(args_1[idx]);
	args_1[idx] = NULL;

	unsigned int j = 0;
	for (unsigned int i = idx + 1; i < num; i++, j++) {
		strcpy(args_2[j], args[i]);
	}

	free(args_2[j]);
	args_2[j] = NULL;
}

void handle_pipe(char **args)
{
	char *args_1[MAX_ARG_LEN], *args_2[MAX_ARG_LEN];		
	
	init_args(args, args_1, args_2);
	int fd[2];

	if (pipe(fd) == -1) {
		perror("pipe");
		return;
	}

	if (fork() == 0) {
		close(fd[READ_END]);
		if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1) {
			perror("dup2");
			return;
		}
		execvp(args_1[0], args_1);
		perror(args_1[0]);
		return;
	}

 	if (fork() == 0) {
		close(fd[WRITE_END]);
		if (dup2(fd[READ_END], STDIN_FILENO) == -1) {
			perror("dup2");
			return;
		}
		execvp(args_2[0], args_2);
		perror(args_2[0]);
		return;
	}

	close(fd[0]);
	close(fd[1]);
	wait(NULL);
	wait(NULL);
}

int pipe_idx(char **args)
{
	unsigned int num = num_args();
	for (unsigned int i = 0; i < num; i++) {
		if (strcmp(args[i], "|") == 0) 
			return (int)i;
	}
	return -1;
}

int correct_pipe(char **args) 
{
	int idx = pipe_idx(args);
	if (idx <= 0)
		return 0;

	int num = (int)num_args();
	
	if (idx == num - 1)
		return 0;

	return 1;
}

void alloc_args(char *args[MAX_ARG_LEN])
{
	for (int i = 0; i < MAX_ARGS / 2; i++)
		args[i] = malloc(sizeof(char *));
}
