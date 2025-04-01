/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:58:33 by sepun             #+#    #+#             */
/*   Updated: 2025/04/01 20:33:31 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char	**check_path(char **env, t_pipex *pipex)
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
				perror_and_exit("ft_split", pipex);
			return (new_path);
		}
		i++;
	}
	return (NULL);
}

char	*search_path(char **path, char **cmd)
{
	char	*line;
	char	*join;
	int		i;

	i = 0;
	join = NULL;
	line = NULL;
	if (path == NULL)
		return (NULL);
	while (path[i] != NULL)
	{
		join = ft_strjoin(path[i], "/");
		if (join == NULL)
			perror_and_exit("ft_strjoin", NULL);
		line = ft_strjoin(join, cmd[0]);
		free(join);
		if (line == NULL)
			perror_and_exit("ft_strjoin", NULL);
		if (access(line, F_OK | X_OK) == 0)
			return (line);
		free(line);
		i++;
	}
	return (cmd[0]);
}

void	proccess_file(t_pipex *pipex, char **env, char *file_name)
{
	init_var(pipex);
	pipex->fd[0] = open(file_name, O_RDONLY);
	if (pipex->fd[0] == -1)
		perror_and_exit("open", pipex);
	pipex->env = env;
	pipex->path = check_path(env, pipex);
	if (pipe(pipex->pipe_fd) == -1)
		perror_and_exit("pipe", pipex);
}

void	perror_and_exit(char *str, t_pipex *pipex)
{
	perror(str);
	exit_final(pipex);
	exit(1);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		ft_printf("Invalid number of arguments\n");
		return (1);
	}
	proccess_file(&pipex, env, argv[1]);
	pipex.pid1 = fork();
	if (pipex.pid1 == 0)
		first_command(&pipex, argv[2]);
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
