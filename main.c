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
	ssize_t bytes_read;

	(void)argc;
	(void)argv;

	buf = NULL;

	write(1, "$ ", 2);

	while (1)
	{
		bytes_read = getline(&buf, &n, stdin);
		if (bytes_read == -1)
			break;

		if (strcmp(buf, "exit\n") == 0)
			break;

		execute(buf, env);

		write(1, "$ ", 2);
	}

	free(buf);
	return (0);
}
