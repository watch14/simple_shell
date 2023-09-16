#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

char *get_env(char *env_var, char **env)
{
	int i = 0;
	char *key;

	while (env[i])
	{
		key = strtok(env[i], "=");
		if (strcmp(env_var, key) == 0)
			return (strtok(NULL, "\n"));
		i++;
	}
	return (NULL);
}

void add_path(char *a, char *exe, char **env)
{
	char *path = get_env("PATH", env);
	char *token;
	struct stat st;

	token = strtok(path, ":");

	while (token != NULL)
	{
		strcpy(exe, token);
		strcat(exe, "/");
		strcat(exe, a);

		if (stat(exe, &st) == 0)
			return;

		token = strtok(NULL, ":");
	}

	exe[0] = '\0';
}

void handle_special(char *arg)
{
	int len = strlen(arg);
	int k;

	for (k = 0; k < len; k++)
	{
		if (arg[k] == '"' || arg[k] == '\'' || arg[k] == '`' || arg[k] == '\\' || arg[k] == '*' || arg[k] == '&' || arg[k] == '#')
		{
			memmove(arg + k + 1, arg + k, len - k + 1);
			arg[k] = '\\';
			len++;
			k++;
		}
	}
}

void execute(char *command, char **env)
{
	char *argv[20];
	int i = 0, j;

	char *token = strtok(command, " \t\n");
	pid_t pid;

	while (token != NULL)
	{
		argv[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;

	for (j = 0; j < i; j++)
		handle_special(argv[j]);

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		char exe[50];

		add_path(argv[0], exe, env);
		execvp(exe, argv);
		perror("hsh");

		exit(1);
	}
	else
	{
		wait(NULL);
	}
}

int main(int argc, char **argv, char **env)
{
	char *buf;
        size_t n = 0;

	(void)argc;
	(void)argv;

	buf = NULL;

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
