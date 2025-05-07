/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:46:45 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/07 01:23:36 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// cd ~ /

#include "minishell.h"

char	*home_path(t_info *info)
{
	int	i;
	char	*str;

	i = 0;
	while(info->env[i])
	{
		if (ft_strncmp(info->env[i], "HOME=", 5) == 0)
			str = (info->env[i] + 5);
		i++;
	}
	return (str);
}

void	update_oldpwd(t_info *info)
{
	int	i;
	char	*oldpwd;
	char	*pwd;

	i = 0;
	while(info->env[i])
	{
		if (ft_strncmp(info->env[i], "PWD=", 4) == 0)
			pwd = (info->env[i]+ 4);
		i++;
	}
	oldpwd = ft_strjoin("OLDPWD=", pwd);
	i = 0;
	while(info->env[i])
	{
		if (ft_strncmp(info->env[i], "OLDPWD=", 7) == 0)
		{
			free(info->env[i]);
			info->env[i] = oldpwd;
		}
		i++;
	}
}

void	update_pwd(t_info *info)
{
	int	i;
	char	*newpwd;
	char	*join;

	i = 0;
	newpwd = ft_pwd();
	join = ft_strjoin("PWD=", newpwd);
	while(info->env[i])
	{
		if (ft_strncmp(info->env[i], "PWD=", 4) == 0)
		{
			free(info->env[i]);
			info->env[i] = join;
		}
		i++;
	}
}

void	ft_cd(char **args, t_info *info)
{
	char	*home;

	home = home_path(info);
	if (args[2])
		return (ft_printf(2, "MINISHELL: cd: too many arguments\n"), estat(1));
	if (args[1] == NULL || (ft_strcmp(args[1], "~") == 0))
	{
		update_oldpwd(info);
		chdir((const char*)home);
		update_pwd(info);
		return (estat(0));
	}
	else if(args[1])
	{
		if (chdir(args[1]) != 0)
			return (ft_printf(2, "Minishell: cd: %s: %s\n", args[1], strerror(errno)), estat(1));
		else
		{
			update_oldpwd(info);
			update_pwd(info);
			return (estat(0));
		}
	}
	return (estat(1));
}

// int	main(int ac, char **av, char **env)
// {
// 	t_info	info;
// 	char	*args[2];
// 	// int	i;

// 	// i = 0;
// 	(void)ac;
// 	(void)av;
// 	args[0] = "~/th";
// 	args[1] = NULL;
// 	info.env = NULL;
// 	form_env(env, &info);
// 	// while (info.env[i])
// 	// {
// 	// 	printf("%s\n", info.env[i]);
// 	// 	i++;
// 	// }
// 	// printf("\nBEFORE\n\n\nAFTER\n");
// 	ft_cd(args, &info);
// 	// i = 0;
// 	// while (info.env[i])
// 	// {
// 	// 	printf("%s\n", info.env[i]);
// 	// 	fflush(stdout);
// 	// 	i++;
// 	// }
// }

