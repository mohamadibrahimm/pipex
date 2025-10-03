/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 22:12:19 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/03 23:50:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(t_pipex *pipex, char **envp, char **cmd_exc)
{
	int		i;
	char	*cmd;

	i = 0;
	while (pipex->full_dir[i])
	{
		cmd = ft_strjoin(pipex->full_dir[i], "/", cmd_exc[0]);
		if (access(cmd, X_OK) == 0)
			execve(cmd, cmd_exc, envp);
		i++;
		free(cmd);
	}
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return (0);
}

void	pipex_vars(t_pipex *pipex, char **argv)
{
	pipex->status = 0;
	pipex->fd[0] = open(argv[1], O_RDONLY);
	if (pipex->fd[0] < 0)
		perror(argv[1]);
	pipex->fd[1] = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipex->fd[1] < 0)
	{
		perror(argv[4]);
		if (pipex->fd[1] >= 0)
			close(pipex->fd[1]);
	}
}

void	pipex_init(t_pipex *pipex, char **argv, char **envp)
{
	char	*dir;

	dir = get_dir(envp);
	pipex->cmd1 = ft_split(argv[2], ' ');
	pipex->cmd2 = ft_split(argv[3], ' ');
	pipex->full_dir = ft_split(dir, ':');
	free(dir);
	pipex_vars(pipex, argv);
	if (!pipex->cmd1 || !pipex->cmd1[0])
	{
		handle_error(3);
		free_all(pipex);
		free(pipex);
		exit(1);
	}
	if (!pipex->cmd2 || !pipex->cmd2[0])
	{
		handle_error(4);
		free_all(pipex);
		free(pipex);
		exit(1);
	}
}
