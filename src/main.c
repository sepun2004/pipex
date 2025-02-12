/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/02/12 21:19:34 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

// [fichero] [cmd1] [cmd2]  [fichero out]
// <Makefile cmd1 | cmd2 out.txt>
// <Makefile cat | grep  ".c" out.txt>


void pid2_exec(int *fd, int *pipe_fd)
{
	char *cmd[] = {"ls", "-la", NULL};
	dup2(pipe_fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	execve("/bin/ls", cmd, NULL);
	printf("si llegue aqui significa que fallé\n");
	exit(1);
}

void pid1_exec(int *fd, int *pipe_fd)
{
	char *cmd[] = {"sleep", "5", NULL};
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	execve("/bin/sleep", cmd, NULL);
	printf("si llegue aqui significa que fallé\n");
	exit(1);
}

int main(int argc, char **argv, char **env)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	int		fd[2]; // donde fd[0] = infile  fd[1] = outfile
	int		pipe_fd[2]; // donde pipe_fd[0] = leer  pipe_fd[1] = esccribir
/* 	if (argc != 4)
	{
		printf("Error: Numero de argumentos incorrecto\n");
		return -1;
	} */
	fd[0] = open(argv[1], O_RDONLY);
	fd[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pipe(pipe_fd);
	pid1 = fork();
	if (pid1 == 0)
	{
		pid1_exec(fd, pipe_fd);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		pid2_exec(fd, pipe_fd);
	}
	while (waitpid(pid1, &status, 0) > 0)
		waitpid(pid2, &status, 0);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(fd[0]);
	close(fd[1]);
	return (0);
}