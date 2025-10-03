/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 19:40:23 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/04 00:23:53 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(t_pipex *pipex)
{
	free_split(pipex->cmd1);
	free_split(pipex->cmd2);
	free_split(pipex->full_dir);
}

ssize_t	handle_error(int i)
{
	if (i == 0)
		return (write(1, "Usage : ./pipex infile \"cmd1\" \"cmd2\" outfile\n",
				45));
	else if (i == 1)
		return (write(1, "Error : non existing infile or denied access.\n",
				46));
	else if (i == 3)
		return (write(1, "Error : invalid first command\n", 30));
	else if (i == 4)
		return (write(1, "Error : invalid second command\n", 31));
	return (0);
}

int	countwords(const char *s, char c)
{
	int	i;
	int	count;
	int	isword;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (s[i])
	{
		isword = 0;
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			isword = 1;
		if (isword == 1)
			count++;
		i++;
	}
	return (count);
}

char	*allocate(const char *s, int start, int len)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	while (s[start] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**big;
	int		start;
	int		end;
	int		i;
	int		j;

	i = 0;
	j = 0;
	big = (char **)malloc(sizeof(char *) * (countwords(s, c) + 1));
	if (!big)
		return (NULL);
	while (s[i])
	{
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			start = i;
		if ((s[i] != c && s[i + 1] == c) || (s[i] != c && s[i + 1] == '\0'))
		{
			end = i;
			if (j < countwords(s, c))
				big[j++] = allocate(s, start, end - start + 1);
		}
		i++;
	}
	big[j] = NULL;
	return (big);
}
// int main(void)
// {
// 	char **big;
// 	int i = 0;

// 	big = ft_split("wc -l", ' ');
// 	while (big[i])
// 	{
// 		printf("big[%d] : %s\n", i, big[i]);
// 		i++;
// 	}
// }