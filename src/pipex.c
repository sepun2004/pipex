/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/03/31 18:42:00 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"


// ./a.out [fichero] [cmd1] [cmd2]  [fichero out]
// <Makefile cmd1 | cmd2 out.txt>
// <Makefile cat | grep  ".c" out.txt>

// r w x
// 777 == 111 111 111
// 644 == 110 100 100

void second_command(pipex_t *pipex, char *cmd, char *file_name)
{
	pipex->fd[1] = open(file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->fd[1] == -1)
		print_error_and_exit("Error: No se pudo abrir el archivo", pipex);
	if (dup2(pipex->pipe_fd[0], STDIN_FILENO) == -1)
		print_error_and_exit("Error: en el dup2 pipe_fd[0]\n", pipex);
	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error: en el dup2 fd1\n", pipex);
	close(pipex->fd[1]);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	// dprintf(2, "ingresa por el segundo comando \n");
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
				print_error_and_exit("Error: No se pudo dividir la variable de entorno PATH\n", pipex);
			return (new_path);
		}
		i++;
	}
	// print_error_and_exit("Error: No se encontro la variable de entorno PATH\n", pipex);
	return (NULL);
}

// ./ls 
// /bin/ls
// ls

char *search_path(char **path, char **cmd)
{
	// dprintf(2, "ingresa a search_path\n");
	char *line;
	char *join;
	int i;

	i = 0;
	join = NULL;
	if (path == NULL)
		return (NULL);
	while (path[i] != NULL)
	{
		join = ft_strjoin(path[i], "/");
		if (join == NULL)
			print_error_and_exit("Error: No se pudo hacer join\n", NULL);
		line = ft_strjoin(join, cmd[0]);
		if (line == NULL)
			print_error_and_exit("Error: No se pudo hacer join\n", NULL);
		if (access(line, F_OK | X_OK) == 0)
			return (line);
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
	if (!str)
		print_error_and_exit("Error: No se pudo hacer split", pipex);
	if (cmd[0] == '.' && cmd[1] == '/') //./ls
	{
		dprintf(2, "Es una ruta ./ relativa\n");
		if (access(str[0], F_OK | X_OK) == -1 || execve(str[0], str, pipex->env) == -1)
			print_error_and_exit("Error: el comando ./", pipex);
	}
	else if (cmd[0] == '/')
	{
		dprintf(2, "Es una ruta / absoluta\n");
		if (access(str[0], F_OK | X_OK) == -1 || execve(str[0], str, pipex->env) == -1)
			print_error_and_exit("Error: el comando /", pipex);
	}
	else
	{
		temp = search_path(pipex->path, &str[0]);
		dprintf(2, "temp: %s\n", temp);
		if (temp == NULL || execve(temp, str, pipex->env) == -1)
			print_error_and_exit("Error: No se pudo ejecutar el comando ----", pipex);
	}
}

void first_command(pipex_t *pipex, char *cmd, char *file_name)
{
	pipex->fd[0] = open(file_name, O_RDONLY);
	if (pipex->fd[0] == -1)
		print_error_and_exit("Error: No se pudo abrir el archivo", pipex);
	if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
		print_error_and_exit("Error: nose dup2 fd[0] firts command", pipex);
	close(pipex->fd[0]);
	if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
		print_error_and_exit("Error: dup2 pipe_fd[1] first command", pipex);
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	execute(pipex, cmd);
}

void free_path(char **path)
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


int main(int argc, char **argv, char **env)
{
	pipex_t pipex;
	
	if (argc != 5)
		print_error_and_exit("Error: Argumentos invalidos", &pipex);
	// check_comand(argc, cmd);
	pipex.env = env;
	pipex.path = check_path(env, &pipex);
	if (pipe(pipex.pipe_fd) == -1)
		print_error_and_exit("Error: No se pudo crear el pipe", &pipex);
	pipex.pid1 = fork();
	if (pipex.pid1 == 0)
		first_command(&pipex, argv[2], argv[1]);
	// dprintf(2, "aqui inicia el segundo comando\n");
	pipex.pid2 = fork();
	if (pipex.pid2 == 0)
		second_command(&pipex, argv[3], argv[4]);
	close(pipex.pipe_fd[0]);
	close(pipex.pipe_fd[1]);
	free_path(pipex.path);
	waitpid(pipex.pid1, &pipex.status, 0);
	waitpid(pipex.pid2, &pipex.status, 0);

	return (0);
}
