/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:50:25 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 20:33:19 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	execute(t_pipex *pipex, char *cmd)
{
	char	**str;
	char	*temp;

	temp = NULL;
	str = ft_split(cmd, ' ');
	if (str == NULL)
		perror_and_exit("ft_split", pipex);
	if (cmd[0] == '.' && cmd[1] == '/')
	{
		if (access(str[0], F_OK | X_OK) == -1
			|| execve(str[0], str, pipex->env) == -1)
			(free_double(str), perror_and_exit("Error in ./", pipex));
	}
	else if (cmd[0] == '/')
	{
		if (access(str[0], F_OK | X_OK) == -1
			|| execve(str[0], str, pipex->env) == -1)
			(free_double(str), perror_and_exit("Error in /", pipex));
	}
	else
	{
		temp = search_path(pipex->path, &str[0]);
		if (temp == NULL || execve(temp, str, pipex->env) == -1)
			(free_double(str), perror_and_exit("Error in search_path", pipex));
	}
}

void	first_command(t_pipex *pipex, char *cmd)
{
	if (ft_strnstr(cmd, "exit", 10) != NULL)
	{
		ft_putstr_fd("exit\n", 2);
		(exit_final(pipex), exit(0));
	}
	if (!cmd || ft_strlen(cmd) <= 1)
	{
		perror_and_exit("Not command", pipex);
	}
	if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
		perror_and_exit("dup2", pipex);
	close(pipex->fd[0]);
	if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
		perror_and_exit("dup2", pipex);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	if (pipex->fd[1] != -1)
		close(pipex->fd[1]);
	execute(pipex, cmd);
}

void	second_command(t_pipex *pipex, char *cmd, char *file_name)
{
	if (ft_strnstr(cmd, "exit", 4) != NULL)
	{
		ft_putstr_fd("exit\n", 2);
		(exit_final(pipex), exit(0));
	}
	if (!cmd || ft_strlen(cmd) <= 1)
		perror_and_exit("Not command", pipex);
	pipex->fd[1] = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->fd[1] == -1)
		perror_and_exit("open", pipex);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
		perror_and_exit("dup2 pipe_fd", pipex);
	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		perror_and_exit("dup2 fd", pipex);
	close(pipex->fd[1]);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	close(pipex->fd[0]);
	execute(pipex, cmd);
}
