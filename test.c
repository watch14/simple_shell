#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void add_path(char *a, char *exe)
{
	char *path = "/bin/";

	strcpy(exe, path);
	strcat(exe, a);
}

void execute(char *command, char **env)
{
	char *cmd = strtok(command, " \t\n");
	char *argv[20];
	int i = 0;

	while (cmd != NULL)
	{
		argv[i++] = cmd;
		cmd = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;

	char exe[50];

	add_path(argv[0], exe);

	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)/* Child process*/
	{
		execve(exe, argv, env);
		perror("hsh");
		exit(1);
	}
	else/*Parent process*/
	{
		wait(NULL);
	}
}

int main(int ac, char **av, char **env)
{
	char *buf = NULL;
	size_t n = 0;

	printf("$ ");
	getline(&buf, &n, stdin);
	while (buf)
	{
		if (strcmp(buf, "exit\n") == 0)
			break;

		execute(buf, env);

		printf("$ ");
		getline(&buf, &n, stdin);
	}
	free(buf);
	return (0);
}
