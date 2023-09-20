#include "main.h"


/**
 * get_env - Get the value of an environment variable.
 * @env_var: The name of the environment variable to retrieve.
 * @env: The array of environment variables.
 *
 * Return: The value of the specified environment, or NULL if not found.
 */

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

/**
 * add_path - Add the path of the executable to the command.
 * @a: The command or executable.
 * @exe: The buffer to store the full path.
 * @env: The environment variables array.
 */

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

/**
 * handle_special - Handle special characters in an argument.
 * @arg: The argument to process for special characters.
 */

void handle_special(char *arg)
{
	int len = strlen(arg);
	int k;

	if (arg == NULL)
	{
		return;
	}

	for (k = 0; k < len; k++)
	{
		if (arg[k] == '"' || arg[k] == '\'' ||
				arg[k] == '`' || arg[k] == '\\' ||
				arg[k] == '*' || arg[k] == '&' || arg[k] == '#')
		{
			memmove(arg + k + 1, arg + k, len - k + 1);
			arg[k] = '\\';
			len++;
			k++;
		}
	}
}

/**
 * execute - Execute a command in a new process.
 * @command: The command to execute.
 * @env: The environment variables array.
 * @line_number: The line number of the command in the shell script.
 */

void execute(char *command, char **env, int line_number)
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
		if (exe[0] == '\0')
		{
			fprintf(stderr, "hsh: %d: %s: not found\n", line_number, argv[0]);
			exit(1);
		}
		execve(exe, argv, env);
		fprintf(stderr, "hsh: %d: %s: not found\n", line_number, argv[0]);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}
