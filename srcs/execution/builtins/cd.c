/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malatini <malatini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 15:46:40 by malatini          #+#    #+#             */
/*   Updated: 2021/09/30 14:38:02 by malatini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
** Fonction pour la norme + gestion des erreurs
* @author: malatini
*/
int	sub_execute_cd_args(t_cmd_elem *elem, t_mem *mem, char *current_path)
{
	char	*new_pwd;

	new_pwd = NULL;
	if (chdir(elem->args[1]) == -1)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(elem->args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		push_ret_elem(mem, 127);
		return (1);
	}
	else
	{
		new_pwd = getcwd(NULL, 0);
		replace_value_in_env("OLDPWD", current_path, mem);
		replace_value_in_env("PWD", new_pwd, mem);
		push_ret_elem(mem, 0);
		free(new_pwd);
		return (0);
	}
	return (0);
}

/**
** Gestion du cd quand il y a plusieurs arguments
* @author: malatini
**/
int	execute_cd_args(t_cmd_elem *elem, t_mem *mem, char *current_path)
{
	int		ret;
	char	*new_pwd;

	ret = 0;
	new_pwd = NULL;
	if (elem->args_len > 2)
		return (d_err_p_ret("bash: cd: too many arguments\n", mem, 1, 2));
	if (elem->args[1][0] != '-')
		return (sub_execute_cd_args(elem, mem, current_path));
	else
	{
		new_pwd = find_value_in_env("OLDPWD", mem);
		replace_value_in_env("PWD", new_pwd, mem);
		replace_value_in_env("OLDPWD", current_path, mem);
		chdir(new_pwd);
		push_ret_elem(mem, 0);
		return (0);
	}
	if (new_pwd)
		free(new_pwd);
	new_pwd = NULL;
	return (ret);
}

/**
** Va permettre d'effectuer un cd sans arg en modifiant les
** variables d'environnement qui vont bien.
* @author: malatini
*/
int	execute_cd_no_arg(t_cmd_elem *elem, t_mem *mem)
{
	char	*home;
	char	*current_wd;

	(void)elem;
	home = find_value_in_env("HOME", mem);
	current_wd = getcwd(NULL, 0);
	chdir(home);
	set_value_in_env("PWD", home, mem);
	set_value_in_env("OLDPWD", current_wd, mem);
	if (home)
	{
		free(home);
		home = NULL;
	}
	if (current_wd)
	{
		free(current_wd);
		current_wd = NULL;
	}
	push_ret_elem(mem, 0);
	return (0);
}

/**
** Fonction principale pour gerer le builtin cd
* @author: malatini
*/
int	ft_exec_cd(t_cmd_elem *elem, t_mem *mem)
{
	int		i;
	char	*current_path;

	i = 0;
	current_path = getcwd(NULL, 0);
	if (elem->args_len == 1)
		i = execute_cd_no_arg(elem, mem);
	else
		i = execute_cd_args(elem, mem, current_path);
	free(current_path);
	return (i);
}
