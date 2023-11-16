#include "holberton.h"

/**
 * split_line - arrange an array of pointers to tokens of
 * a buffer with a delimiter.
 * @buffer: buffer to be tokened
 * @delimiter: delimiter to match the buffer
 *
 * Return: pointer to an array of pointers to the tokens
 */

char **split_line(char *buffer, char *delimiter)
{
	char **tokens = NULL;
	size_t i = 0, mcount = 10;

	if (buffer == NULL)
		return (NULL);
	tokens = malloc(sizeof(char *) * mcount);
	if (tokens == NULL)
	{
		perror("Fatal Error");
		return (NULL);
	}
	while ((tokens[i] = _strtok(buffer, delimiter)) != NULL)
	{
		i++;
		if (i == mcount)
		{
			tokens = _realloc(tokens, &mcount);
			if (tokens == NULL)
			{
				perror("Fatal Error");
				return (NULL);
			}
		}
		buffer = NULL;
	}
	return (tokens);
}
