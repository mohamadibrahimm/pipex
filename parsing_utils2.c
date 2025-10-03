/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:27:53 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/01 22:24:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *str)
{
	char	*dup;
	int		i;

	i = 0;
	dup = (char *)malloc(sizeof (char) * (ft_strlen(str) + 1));
	if (!dup)
		return (0);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*get_dir(char **env)
{
	int		i;
	char	*dir;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			dir = ft_strdup(env[i] + 5);
		i++;
	}
	return (dir);
}

char	*ft_strjoin(char const *s1, char const *s2, char const *s3)
{
	char	*new;
	int		i;
	int		j;
	int		w;

	if (!s1 && !s2)
		return (NULL);
	new = (char *)malloc(sizeof (char) * (ft_strlen(s1)
				+ ft_strlen(s2) + ft_strlen(s3) + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	w = 0;
	while (s1 && s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
		new[i++] = s2[j++];
	while (s3 && s3[w] != '\0')
		new[i++] = s3[w++];
	new[i] = '\0';
	return (new);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// int main (void)
// {
// 	char *s1 = "/usr/bin";
// 	char *s2 = "/";
// 	char *s3 = "ls";
// 	char *full = ft_strjoin(s1, s2, s3);
// 	printf("%s", full);
// }
