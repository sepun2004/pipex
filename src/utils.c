/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 10:39:51 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 20:33:58 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	init_var(t_pipex *pipex)
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

void	free_double(char **path)
{
	int	i;

	i = 0;
	while (path[i] != NULL)
	{
		free(path[i]);
		i++;
	}
	free(path);
}

void	print_error_and_exit(char *msg, t_pipex *pipex)
{
	ft_putstr_fd(msg, 2);
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
	exit(1);
}

void	exit_final(t_pipex *pipex)
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
