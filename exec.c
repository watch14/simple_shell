#include "main.h"

void execute(char *command, char **env, int line_number);
void execute_command(char *command, char **argv, char **env, int line_number);
void execute_path(char *exe, char **argv, char **env, int line_number);

/**
 * execute - Execute a command in a new process.
 * @command: The command to execute.
 * @env: The environment variables array.
 * @line_number: The line number of the command in the shell script.
 */
void execute(char *command, char **env, int line_number)
{
	char *argv[20];
	int i = 0;
	char *token = strtok(command, " \t\n");

	pid_t pid;

	while (token != NULL)
	{
		argv[i++] = token;
		token = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;

	for (int j = 0; j < i; j++)
		handle_special(argv[j]);

	if (strchr(argv[0], '/') != NULL)
	{
		execute_command(argv[0], argv, env, line_number);
	}
	else
	{
		char exe[50];

		add_path(argv[0], exe, env);

		if (exe[0] == '\0')
		{
			fprintf(stderr, "hsh: %d: %s: not found\n", line_number, argv[0]);
			return;
		}

		execute_path(exe, argv, env, line_number);
	}
}

/**
 * execute_command - Execute a command with a full path.
 * @command: The command to execute.
 * @argv: The array of command arguments.
 * @env: The environment variables array.
 * @line_number: The line number of the command in the shell script.
 */
void execute_command(char *command, char **argv, char **env, int line_number)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		execve(command, argv, env);
		fprintf(stderr, "hsh: %d: %s: execution failed\n", line_number, argv[0]);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}

/**
 * execute_path - Execute a command found in the PATH environment variable.
 * @exe: The full path to the executable.
 * @argv: The array of command arguments.
 * @env: The environment variables array.
 * @line_number: The line number of the command in the shell script.
 */
void execute_path(char *exe, char **argv, char **env, int line_number)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		execve(exe, argv, env);
		fprintf(stderr, "hsh: %d: %s: execution failed\n", line_number, argv[0]);
		exit(1);
	}
	else
	{
		wait(NULL);
	}
}
