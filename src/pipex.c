/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/03/31 21:19:11 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void init_var(pipex_t *pipex)
{
	pipex->pipe_fd[0] = -1;
	pipex->pipe_fd[1] = -1;
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->pid1 = -1;
	pipex->pid2 = -1;
	pipex->status = 0;
	pipex->path = NULL;
	pipex->env = NULL;
}

void free_double(char **path)
{
	int i;

	i = 0;
	while (path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}


void second_command(pipex_t *pipex, char *cmd, char *file_name)
{
	if (ft_strnstr(cmd, "exit", 4) != NULL)
	{
		dprintf(2, "exit\n");
		(exit_final(pipex), exit(0));
	}
	pipex->fd[1] = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->fd[1] == -1)
		print_error_and_exit("Could not open the file", pipex);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
		print_error_and_exit("Error in pipe_fd[0] \n", pipex);
	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error in fd1\n", pipex);
	close(pipex->fd[1]);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	close(pipex->fd[0]);
	execute(pipex, cmd);
}

void  print_error_and_exit(char *msg, pipex_t *pipex)
{
	dprintf(2, "%s\n", msg);
	if(pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->fd[0] != -1)	
		close(pipex->fd[0]);
	if (pipex->fd[1] != -1)
		close(pipex->fd[1]);
	if (pipex->path != NULL)
		free_double(pipex->path);
	exit(1);
}

char	**check_path(char **env, pipex_t *pipex)
{
	char	**new_path;
	char	*path;
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		path = ft_strnstr(env[i], "PATH", 4);
		if (path != NULL)
		{
			new_path = ft_split(path + 5, ':');
			if (new_path == NULL)
				print_error_and_exit("Error in new_path\n", pipex);
			return (new_path);
		}
		i++;
	}
	return (NULL);
}

char *search_path(char **path, char **cmd)
{
	char *line;
	char *join;
	int i;

	i = 0;
	join = NULL;
	line = NULL;
	if (path == NULL)
		return (NULL);
	while (path[i] != NULL)
	{
		join = ft_strjoin(path[i], "/");
		if (join == NULL)
			print_error_and_exit("Error in join\n", NULL);
		line = ft_strjoin(join, cmd[0]);
		free(join);
		if (line == NULL)
			print_error_and_exit("Error in line\n", NULL);
		if (access(line, F_OK | X_OK) == 0)
			return (line);
		free(line);
		i++;
	}
	return (cmd[0]);
}

void execute(pipex_t *pipex, char *cmd)
{
	char **str;
	char *temp;

	temp = NULL;
	str = ft_split(cmd, ' ');
	if (str == NULL)
		print_error_and_exit("Could not get the command", pipex);
	if (cmd[0] == '.' && cmd[1] == '/') //./ls
	{
		if (access(str[0], F_OK | X_OK) == -1 || execve(str[0], str, pipex->env) == -1)
			print_error_and_exit("Error in ./", pipex);
	}
	else if (cmd[0] == '/')
	{
		if (access(str[0], F_OK | X_OK) == -1 || execve(str[0], str, pipex->env) == -1)
			print_error_and_exit("Error in /", pipex);
	}
	else
	{
		temp = search_path(pipex->path, &str[0]);
		if (temp == NULL || execve(temp, str, pipex->env) == -1)
			print_error_and_exit("Error in search_path", pipex);
	}
}

void first_command(pipex_t *pipex, char *cmd, char *file_name)
{
	(void)file_name;
	if (ft_strnstr(cmd, "exit", 4) != NULL)
	{
		dprintf(2, "exit\n");
		(exit_final(pipex), exit(0));
	}
	if (!cmd || ft_strlen(cmd) <= 1)
	{
		dprintf(2, "No command\n");
		exit(1);
	}
	if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
		print_error_and_exit("Error: nose dup2 fd[0] firts command", pipex);
	close(pipex->fd[0]);
	if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error: dup2 pipe_fd[1] first command", pipex);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	if (pipex->fd[1] != -1)
		close(pipex->fd[1]);
	execute(pipex, cmd);
}

void exit_final(pipex_t *pipex)
{
	if (pipex->pipe_fd[1] != -1)
		close(pipex->pipe_fd[1]);
	if (pipex->pipe_fd[0] != -1)
		close(pipex->pipe_fd[0]);
	if (pipex->fd[0] != -1)
		close(pipex->fd[0]);
	if (pipex->fd[1] != -1)
		close(pipex->fd[1]);
	if (pipex->path != NULL)
		free_double(pipex->path);
}

void proccess_file(pipex_t *pipex, char **env, char *file_name)
{
	init_var(pipex);
	pipex->fd[0] = open(file_name, O_RDONLY);
	if (pipex->fd[0] == -1)
		print_error_and_exit("Could not open the file", pipex);
	pipex->env = env;
	pipex->path = check_path(env, pipex);
	if (pipe(pipex->pipe_fd) == -1)
		print_error_and_exit("Error in pipe", pipex);
	
}

int main(int argc, char **argv, char **env)
{
	pipex_t	pipex;

	if (argc != 5)
	{
		dprintf(2, "Invalid number of arguments\n");
		return (1);
	}
	proccess_file(&pipex, env, argv[1]);
	pipex.pid1 = fork();
	if (pipex.pid1 == 0)
		first_command(&pipex, argv[2], argv[1]);
	pipex.pid2 = fork();
	if (pipex.pid2 == 0)
		second_command(&pipex, argv[3], argv[4]);
	close(pipex.pipe_fd[1]);
	close(pipex.pipe_fd[0]);
	close(pipex.fd[0]);
	waitpid(pipex.pid1, &pipex.status, 0);
	waitpid(pipex.pid2, &pipex.status, 0);
	return (0);
}
