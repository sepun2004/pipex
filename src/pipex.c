/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 10:52:00 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
	exit_final(&pipex);
	return (0);
}
