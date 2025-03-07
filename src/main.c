/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/03/07 19:44:22 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"


// ./a.out [fichero] [cmd1] [cmd2]  [fichero out]
// <Makefile cmd1 | cmd2 out.txt>
// <Makefile cat | grep  ".c" out.txt>


// void pid2_exec(int *fd, int *pipe_fd)
// {
// 	char *cmd[] = {"ls", "-la", NULL};
// 	dup2(pipe_fd[0], STDIN_FILENO);
// 	close(fd[0]);
// 	dup2(fd[1], STDOUT_FILENO);
// 	close(fd[1]);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);
// 	execve("/bin/ls", cmd, NULL);
// 	printf("si llegue aqui significa que fallé\n");
// 	exit(1);
// }

// void pid1_exec(int *fd, int *pipe_fd)
// {
// 	int status;
// 	char *cmd[] = {"sleep", "5", NULL};
// 	dup2(fd[0], STDIN_FILENO);
// 	close(fd[0]);
// 	close(fd[1]);
// 	dup2(pipe_fd[1], STDOUT_FILENO);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);
// 	status = execve("/bin/sleep", cmd, NULL);
// 	printf("status: %d\n", status);
// 	printf("si llegue aqui significa que fallé\n");
// 	exit(1);
// }

void  print_error_and_exit(char *msg)
{
	ft_printf("%s\n", msg);
	exit(1);
}

int ft_len(char **cmd)
{
	int i;

	i = 0;
	while (cmd[i] != NULL)
	{
		// ft_printf("cmd[%d]: %s\n", i, cmd[i]);
		i++;
	}
	return (i);
}

char	**check_path(char **env)
{
	int i;
	char *path;
	char **new_path;

	i = 0;
	while (env[i] != NULL)
	{
		path = ft_strnstr(env[i], "PATH", 4);
		if (path != NULL)
		{
			new_path = ft_split(path, ':');
			if (new_path == NULL)
				print_error_and_exit("Error: No se pudo dividir la variable de entorno PATH\n");
			return (new_path);
		}
		i++;
	}
	print_error_and_exit("Error: No se encontro la variable de entorno PATH\n");
	return (NULL);
}

// void	check_comand(int argc, char **cmd)
// {
// 	if (argc != 5)
// 		print_error_and_exit("Error: Numeros de argumentos invalidos\n");
// 	printf("----cmd: %d\n", ft_len(cmd));
// 	if (ft_len(cmd) == 2)
// 	{
// 		printf("ingreso\n");
// 		printf("cmd[0]: %s\n", cmd[0]);
// 		printf("cmd[1]: %s\n", cmd[1]);
// 		if (access(cmd[0], X_OK) == -1 || access(cmd[1], X_OK) == -1)
// 			print_error_and_exit("Error: El comando no es valido\n");
		
// 	}
// }



int main(int argc, char **argv, char **env)
{
	pipex_t pipex;
	
	pipex.cmd[0] = argv[2];  // El primer comando de los argumentos
	pipex.cmd[1] = argv[3];  // El segundo comando de los argumentos
	pipex.cmd[2] = NULL;
	(void)argc;
	(void)pipex.cmd;
	// check_comand(argc, cmd);
	pipex.path = check_path(env);

	pipex.fd[0] = open(argv[1], O_RDONLY);
	if (pipex.fd[0] == -1)
		print_error_and_exit("Error: No se pudo abrir el archivo");
	pipex.fd[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.fd[1] == -1)
		print_error_and_exit("Error: No se pudo crear el archivo");
	if(pipe(pipex.pipe_fd) == -1)
		print_error_and_exit("Error: No se pudo crear el pipe");
	pipex.pid1 = fork();
	if (pipex.pid1 == 0)
	{
		printf("pid1\n");
		printf("este es el hijo\n");
		// pid1_exec(fd, pipe_fd);
	}
	pipex.pid2 = fork();
	if (pipex.pid2 == 0)
	{
		printf("pid2\n");
		printf("este es el hijo\n");
		// pid2_exec(fd, pipe_fd);
	}
	while (waitpid(pipex.pid1, &pipex.status, 0) > 0)
		waitpid(pipex.pid2, &pipex.status, 0);
	close(pipex.pipe_fd[0]);
	close(pipex.pipe_fd[1]);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	return (0);
}
