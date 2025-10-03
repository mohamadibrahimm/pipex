/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:40:20 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/03 23:51:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_fds(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	child1(int pipefd[2], t_pipex *pipex, char **envp, int fd)
{
	if (fd < 0)
	{
		close_fds(pipefd);
		free_all(pipex);
		free(pipex);
		exit(1);
	}
	dup2(pipefd[1], 1);
	if (fd >= 0)
		dup2(fd, 0);
	close_fds(pipefd);
	execute(pipex, envp, pipex->cmd1);
	free_all(pipex);
	free(pipex);
	exit(127);
}

void	child2(int pipefd[2], t_pipex *pipex, char **envp, int fd)
{
	if (fd < 0)
	{
		close_fds(pipefd);
		free_all(pipex);
		free(pipex);
		printf("in the child 2 the fd is %i", fd);
		exit(1);
	}
	dup2(pipefd[0], 0);
	if (fd >= 0)
		dup2(fd, 1);
	close(fd);
	close_fds(pipefd);
	execute(pipex, envp, pipex->cmd2);
	free_all(pipex);
	free(pipex);
	exit(127);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*pipex;
	int		exit_code;

	if (argc != 5)
		return (handle_error(0), 1);
	pipex = malloc(sizeof(t_pipex));
	pipex_init(pipex, argv, envp);
	pipe(pipex->pipefd);
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
		child1(pipex->pipefd, pipex, envp, pipex->fd[0]);
	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
		child2(pipex->pipefd, pipex, envp, pipex->fd[1]);
	close_fds(pipex->pipefd);
	waitpid(pipex->pid1, NULL , 0);
	waitpid(pipex->pid2, &pipex->status , 0);
	close(pipex->fd[1]);
	close(pipex->fd[0]);
	exit_code = (pipex->status >> 8);
	free_all(pipex);
	free(pipex);
	return (exit_code);
}
