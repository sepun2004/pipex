/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:50:25 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 12:17:14 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
    dprintf(2, "----first command----\n");
    dprintf(2, "cmd: %s\n", cmd);
    // dprintf(2, "ft_strnstr: %s\n", ft_strnstr(cmd, "exit", 5));
	if (ft_strnstr(cmd, "exit", 10) != NULL)
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
		print_error_and_exit("Error in dup2 fd[0] firts command", pipex);
	close(pipex->fd[0]);
	if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error in dup2 pipe_fd[1] first command", pipex);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	if (pipex->fd[1] != -1)
		close(pipex->fd[1]);
	execute(pipex, cmd);
}

void second_command(pipex_t *pipex, char *cmd, char *file_name)
{
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
	pipex->fd[1] = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->fd[1] == -1)
		print_error_and_exit("Could not open file second command\n", pipex);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
		print_error_and_exit("Error in pipe_fd[0] second command\n", pipex);
	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error in fd1\n", pipex);
	close(pipex->fd[1]);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	close(pipex->fd[0]);
	execute(pipex, cmd);
}
