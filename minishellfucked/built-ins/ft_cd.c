/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:46:45 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/08 16:01:25 by tkurukul         ###   ########.fr       */
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
			str = ft_strdup(info->env[i] + 5);
		i++;
	}
	return (str);
}


void	update_oldpwd(char **matrix)
{
	int	i;
	char	*oldpwd;
	char	*pwd;

	i = 0;
	while(matrix[i])
	{
		if (ft_strncmp(matrix[i], "PWD=", 4) == 0)
			pwd = ft_strdup(matrix[i]+ 4);
		i++;
	}
	oldpwd = ft_strjoin("OLDPWD=", pwd);
	if (oldpwd == NULL)
		return;
	free(pwd);
	i = 0;
	while(matrix[i])
	{
		if (ft_strncmp(matrix[i], "OLDPWD=", 7) == 0)
		{
			free(matrix[i]);
			matrix[i] = oldpwd;
			return ;
		}
		i++;
	}
}

void	update_pwd(char **matrix)
{
	int	i;
	char	*newpwd;
	char	*join;

	i = 0;
	newpwd = ft_pwd();
	join = ft_strjoin("PWD=", newpwd);
	if (join == NULL)
		return;
	while(matrix[i])
	{
		if (ft_strncmp(matrix[i], "PWD=", 4) == 0)
		{
			free(matrix[i]);
			matrix[i] = join;
			break;
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
		chdir((const char*)home);
		update_oldpwd(info->env);
		update_pwd(info->env);
		return (free(home), estat(0));
	}
	else if(args[1])
	{
		if (chdir(args[1]) != 0)
			return (ft_printf(2, "Minishell: cd: %s: %s\n", args[1], strerror(errno)), estat(1));
		update_oldpwd(info->env);
		update_pwd(info->env);
		return (free(home), estat(0));
	}
	return (free(home), estat(1));
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

