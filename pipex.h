/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:11:58 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/03 22:46:49 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

typedef struct s_pipex
{
	char	**cmd1;
	char	**cmd2;
	char	**full_dir;
	int		status;
	int		fd[2];
	int		pipefd[2];
	int		pid1;
	int		pid2;
}	t_pipex;

//parsing_utils
void	free_all(t_pipex *pipex);
ssize_t	handle_error(int i);
int		countwords(const char *s, char c);
char	*allocate(const char *s, int start, int len);
char	**ft_split(char const *s, char c);

//parsing_utils2
char	*get_dir(char **env);
char	*ft_strjoin(char const *s1, char const *s2, char const *s3);
void	free_split(char **split);
char	*ft_strdup(const char *str);

//main
void	child2(int pipefd[2], t_pipex *pipex, char **envp, int fd);
void	child1(int *pipefd, t_pipex *pipex, char **envp, int fd);
void	close_fds(int *fd);

//init
void	pipex_vars(t_pipex *pipex, char **argv);
void	pipex_init(t_pipex *pipex, char **argv, char **envp);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	execute(t_pipex *pipex, char **envp, char **cmd_exc);

#endif