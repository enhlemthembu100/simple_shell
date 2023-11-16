#include "holberton.h"

/**
 * is_builtin - checks if entered command is a builtin
 * @vbles: pointer to a structure of shell variables
 * Return: pointer to the function or NULL
 */

void (*is_builtin(variables_t *vbles))(variables_t *vbles)
{
	unsigned int i;
	builtin_t check[] = {
		{"exit", new_exit},
		{"cd", new_cd},
		{"env", new_env},
		{NULL, NULL}};

	for (i = 0; check[i].function != NULL; i++)
	{
		if (_strcmpr(vbles->arguments[0], check[i].name) == 0)
			break;
	}
	if (check[i].function != NULL)
		check[i].function(vbles);
	return (check[i].function);
}

/**
 * new_cd - change current directory
 * @vbles: pointer to structure of shell variables
 * Return: void
 */

void new_cd(variables_t *vbles)
{
	if (vbles->arguments[1] == NULL)
	{
		perror("Error: expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(vbles->arguments[1]) != 0)
			perror("Simple shell ");
	}
}

/**
 * new_exit - exit program
 * @vbles: pointer to shell variables
 * Return: void
 */

void new_exit(variables_t *vbles)
{
	int status;

	if (_strcmpr(vbles->arguments[0], "exit") == 0 && vbles->arguments[1] != NULL)
	{
		status = _atoi(vbles->arguments[1]);
		if (status == -1)
		{
			vbles->status = 2;
			print_error(vbles, ": Not allowed number: ");
			_puts2(vbles->arguments[1]);
			_puts2("\n");
			free(vbles->commands);
			vbles->commands = NULL;
			return;
		}
		vbles->status = status;
	}
	free(vbles->buffer);
	free(vbles->arguments);
	free(vbles->commands);
	free_env(vbles->env);
	exit(vbles->status);
}

/**
 * new_env - prints the current environment
 * @vbles: pointer to struct of shell variables
 * Return: void.
 */

void new_env(variables_t *vbles)
{
	unsigned int i;

	for (i = 0; vbles->env[i]; i++)
	{
		_puts(vbles->env[i]);
		_puts("\n");
	}
	vbles->status = 0;
}

/**
 * _atoi - converts a string into an integer
 * @str: string to be converted
 *
 * Return: the integer value, or -1 if an error occurs
 */

int _atoi(char *str)
{
	unsigned int i, digits;
	int num = 0, num_test;

	num_test = INT_MAX;
	for (digits = 0; num_test != 0; digits++)
		num_test /= 10;
	for (i = 0; str[i] != '\0' && i < digits; i++)
	{
		num *= 10;
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		if ((i == digits - 1) && (str[i] - '0' > INT_MAX % 10))
			return (-1);
		num += str[i] - '0';
		if ((i == digits - 2) && (str[i + 1] != '\0') && (num > INT_MAX / 10))
			return (-1);
	}
	if (i > digits)
		return (-1);
	return (num);
}
