/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:23 by sepun             #+#    #+#             */
/*   Updated: 2025/03/12 13:41:46 by marvin           ###   ########.fr       */
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

typedef struct pipex_t
{
	char **path;
	char	*cmd[3];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	int		fd[2]; // donde fd[0] = infile  fd[1] = outfile
	int		pipe_fd[2]; // donde pipe_fd[0] = leer  pipe_fd[1] = esccribir
}pipex_t;


#endif