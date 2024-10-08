/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 17:01:41 by malatini          #+#    #+#             */
/*   Updated: 2023/10/13 16:52:37 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*loop_in_paths(t_cmd_elem *elem, char **splitted, char *bin)
{
	int				i;
	int				fd;
	struct stat		s;

	i = 0;
	while (splitted && splitted[i])
	{
		bin = (char *)ft_calloc(sizeof(char), (ft_strlen(splitted[i])
					+ ft_strlen(elem->args[0]) + 2));
		if (!bin)
			return (NULL);
		ft_strcpy(bin, splitted[i]);
		ft_strcat(bin, "/");
		ft_strcat(bin, elem->args[0]);
		fd = open(bin, O_RDONLY);
		if (fstat(fd, &s) != -1)
		{
			close(fd);
			return (bin);
		}
		free(bin);
		i++;
	}
	return (NULL);
}

char	*find_absolute_path(t_cmd_elem *elem, t_mem *mem)
{
	char	*bin;
	char	**splitted_paths;
	char	*env_paths;

	bin = NULL;
	env_paths = find_value_in_env("PATH", mem);
	if (!env_paths)
	{
		return (bin);
	}
	splitted_paths = ft_split(env_paths, ':');
	bin = loop_in_paths(elem, splitted_paths, bin);
	free(env_paths);
	free_tab(splitted_paths);
	return (bin);
}

char	*find_exec(t_cmd_elem *elem, t_mem *mem)
{
	char	*s;
	int		fd;

	s = NULL;
	if (!elem || elem->args_len == 0)
		return (NULL);
	if (is_builtin(elem->args[0]) == true)
		return (ft_strdup("", mem));
	else if (ft_strchr(elem->args[0], '/') || elem->args[0][0] == '.')
	{
		s = ft_strdup(elem->args[0], mem);
		fd = open(s, O_RDONLY, 0644);
		if (fd < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(s, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			push_ret_elem(mem, 127);
			free(s);
			s = NULL;
		}
	}
	else
		s = find_absolute_path(elem, mem);
	return (s);
}
