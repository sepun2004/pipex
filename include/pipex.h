/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:23 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 20:34:52 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h> // open fork dup2 close pipe

typedef struct pipex
{
	char	**path;
	char	**env;
	int		pipe_fd[2]; // donde pipe_fd[0] = leer  pipe_fd[1] = esccribir
	int		fd[2]; // donde fd[0] = infile  fd[1] = outfile
	int		status;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

// Utils.c
void	init_var(t_pipex *pipex);
void	free_double(char **path);
void	print_error_and_exit(char *msg, t_pipex *pipex);
void	exit_final(t_pipex *pipex);

// Pipex.c
void	proccess_file(t_pipex *pipex, char **env, char *file_name);
char	*search_path(char **path, char **cmd);
char	**check_path(char **env, t_pipex *pipex);
void	perror_and_exit(char *str, t_pipex *pipex);

// commands.c
void	execute(t_pipex *pipex, char *cmd);
void	first_command(t_pipex *pipex, char *cmd);
void	second_command(t_pipex *pipex, char *cmd, char *file_name);

#endif