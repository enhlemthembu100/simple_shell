#include "holberton.h"

/**
 * check_for_dir - checks if the command is a part of a path
 * @str: command
 *
 * Return: 1 on success, 0 on failure
 */

int check_for_dir(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
}

/**
 * find_path - finds the PATH variable
 * @env: array of environment variables
 *
 * Return: pointer to the node that contains the PATH, or NULL on failure
 */

char *find_path(char **env)
{
	char *path = "PATH=";
	unsigned int i, j;

	for (i = 0; env[i] != NULL; i++)
	{
		for (j = 0; j < 5; j++)
			if (path[j] != env[i][j])
				break;
		if (j == 5)
			break;
	}
	return (env[i]);
}

/**
 * execute_cwd - executes the command in the current working directory
 * @vbles: pointer to struct of variables
 *
 * Return: 0 on success, 1 on failure
 */
int execute_cwd(variables_t *vbles)
{
	pid_t child_pid;
	struct stat statbuf;

	if (stat(vbles->arguments[0], &statbuf) == 0)
	{
		if (access(vbles->arguments[0], X_OK) == 0)
		{
			child_pid = fork();
			if (child_pid == -1)
				print_error(vbles, NULL);
			if (child_pid == 0)
			{
				if (execve(vbles->arguments[0], vbles->arguments, vbles->env) == -1)
					print_error(vbles, NULL);
			}
			else
			{
				wait(&vbles->status);
				if (WIFEXITED(vbles->status))
					vbles->status = WEXITSTATUS(vbles->status);
				else if (WIFSIGNALED(vbles->status) && WTERMSIG(vbles->status) == SIGINT)
					vbles->status = 1;
				return (0);
			}
			vbles->status = 1;
			return (1);
		}
		else
		{
			print_error(vbles, ": Permission denied\n");
			vbles->status = 1;
		}
			return (0);
	}
	print_error(vbles, ": not found\n");
	vbles->status = 1;
	return (0);
}

/**
 * check_for_path - checks if the command is in the PATH
 * @vbles: structure with all the shell variables
 *
 * Return: void
 */

void check_for_path(variables_t *vbles)
{
	char *path, *path_dup = NULL, *check = NULL;
	unsigned int i = 0, r = 0;
	char **path_tokens;
	struct stat statbuf;

	if (check_for_dir(vbles->arguments[0]))
		r = execute_cwd(vbles);
	else
	{
		path = find_path(vbles->env);
		if (path != NULL)
		{
			path_dup = _strdup(path + 5);
			path_tokens = split_line(path_dup, ":");
			for (i = 0; path_tokens && path_tokens[i]; i++, free(check))
			{
				check = _strcat(path_tokens[i], vbles->arguments[0]);
				if (stat(check, &statbuf) == 0)
				{
					r = path_execute(check, vbles);
					free(check);
					break;
				}
			}
			free(path_dup);
			if (path_tokens == NULL)
			{
				vbles->status = 1;
				new_exit(vbles);
			}
		}
		if (path == NULL || path_tokens[i] == NULL)
		{
			print_error(vbles, ": not found\n");
			vbles->status = 1;
		}
		free(path_tokens);
	}
	if (r == 1)
		new_exit(vbles);
}

/**
 * path_execute - executes a command in the finded path
 * @command: full path to the command
 * @vbles: pointer to struct of shell variables
 *
 * Return: 0 on succcess, 1 on failure
 */
int path_execute(char *command, variables_t *vbles)
{
	pid_t child_pid;

	if (access(command, X_OK) == 0)
	{
		child_pid = fork();
		if (child_pid == -1)
			print_error(vbles, NULL);
		if (child_pid == 0)
		{
			if (execve(command, vbles->arguments, vbles->env) == -1)
				print_error(vbles, NULL);
		}
		else
		{
			wait(&vbles->status);
			if (WIFEXITED(vbles->status))
				vbles->status = WEXITSTATUS(vbles->status);
			else if (WIFSIGNALED(vbles->status) && WTERMSIG(vbles->status) == SIGINT)
				vbles->status = 1;
			return (0);
		}
		vbles->status = 1;
		return (1);
	}
	else
	{
		print_error(vbles, ": Permission denied\n");
		vbles->status = 1;
	}
	return (0);
}
