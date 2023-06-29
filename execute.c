#include "shell.h"

/**
  * execute - executes commands typed into shell
  * command: command
  * @env: environmental variable
  * @num: user command used in error message
  * Return: 0
  */

int execute(char **command, list_t *env, int num)
{
	char *path = NULL;
	int status = 0;
	pid_t pid;

	if (access(command[0], F_OK) == 0 && access(command[0], X_OK) == 0)
	{
		path = command[0];
	}
	else
	{
		path = _which(command[0], env);
		if (path == NULL || access(path, X_OK) != 0)
		{
			not_found(command[0], num, env);
			sh_free_double_ptr(command);
			return (127);
		}
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(path, command, NULL) == -1)
		{
			not_found(command[0], num, env);
			exit_c(command, env);
		}
	}
	else
	{
		wait(&status);
		sh_free_double_ptr(command);
		if (path != command[0])
			free(path);
	}

	return (0);
}
