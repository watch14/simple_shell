#include "main.h"

/**
 * main - Entry point of the simple shell program
 * @argc: Number of command-line arguments (unused)
 * @argv: Array of command-line argument strings (unused)
 * @env: Array of environment variables
 *
 * Return: Always 0 (successful execution)
 */

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
	return 0;
}
