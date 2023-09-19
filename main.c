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
	char *buf = NULL;
	size_t n = 0;

	(void)argc;
	(void)argv;

	buf = NULL;

	while (1)
	{
		printf("$ ");
		fflush(stdout);

		if (getline(&buf, &n, stdin) == -1 || feof(stdin))
		{
			printf("\n");
			break;
		}

		if (strcmp(buf, "exit\n") == 0)
		{
			break;
		}

		execute(buf, env);

		free(buf);
		buf = NULL;
		n = 0;
	}

	free(buf);
	return (0);
}
