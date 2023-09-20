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
	ssize_t getline_status;

	(void)argc;
	(void)argv;

	buf = NULL;

	while (1)
	{
		printf("$ ");
		getline_status = getline(&buf, &n, stdin);
		if (getline_status == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			else
			{
				perror("getline");
				exit(1);
			}
		}

		if (strcmp(buf, "exit\n") == 0)
		{
			break;
		}

		execute(buf, env);
	}

	free(buf);
	return (0);
}
