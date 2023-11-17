#include "holberton.h"

/**
 * main - Entry point for the shell program
 * @argc: Number of arguments
 * @argv: Array of argument strings
 * @env: Array of environment variables
 *
 * Return: Exit status
 */
int main(int argc, char **argv, char **env)
{
	char **newenv;

	/* Assuming you have a function to initialize your shell environment */
	newenv = read_env(env);

	/* Placeholder for the rest of your shell logic */
	/* Replace this with your actual shell implementation */

	/* Don't forget to free the allocated memory when you're done */
	free_env(newenv);

	return (0);
}

/**
 * read_env - Allocate memory space to pass it to the main function
 * @env: Environment passed to main
 *
 * Return: Pointer to the new environment
 */
char **read_env(char **env)
{
	char **newenv = NULL;
	size_t i;

	for (i = 0; env[i] != NULL; i++)
		;
	newenv = malloc(sizeof(char *) * (i + 1));
	if (newenv == NULL)
	{
		perror("Fatal Error");
		exit(1);
	}
	for (i = 0; env[i] != NULL; i++)
		newenv[i] = _strdup(env[i]);
	newenv[i] = NULL;
	return (newenv);
}

/**
 * free_env - Frees the memory previously allocated
 * @env: Shell's environment variables
 *
 * Return: void
 */
void free_env(char **env)
{
	unsigned int i;

	for (i = 0; env[i] != NULL; i++)
		free(env[i]);
	free(env);
}

