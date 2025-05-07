/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 22:13:33 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/07 23:23:55 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// print the args that are give
// the -n flag removes the newline that it prints
// $USER AND STUFF
// $?

void	ft_echo(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[1], "-n") != 0 && i == 0)
			printf("%s", args[i]);
		if (i != 0)
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
		}
		i++;
	}
	if (ft_strcmp(args[1], "-n") != 0)
		printf("\n");
	return (estat(0));
}

// int	main(int ac, char **av)
// {
// 	//t_info	info;
// 	char	*args[4];
// 	//char	*fix[2];
// 	int	i;

// 	(void)i;
// 	(void)ac;
// 	(void)av;
// 	//fix[0] = "fic=1";
// 	//fix[1] = NULL;
// 	args[0] = "-n";
// 	args[1] = "USER";
// 	args[2] = "fic=1";
// 	args[3] = NULL;
// 	//info.env = NULL;
// 	ft_echo(args);
// 	// ft_env(env, &info);
// 	// ft_export(&info, fix);
// 	// ft_unset(&info, args);
// 	// while (info.env[i])
// 	// {
// 	// 	printf("%s\n", info.env[i]);
// 	// 	i++;
// 	// }
// 	// free_mat(info.env);
// 	// printf("\nBEFORE\n\n\nAFTER\n");
// 	// i = 0;
// 	// while (info.env[i])
// 	// {
// 	// 	printf("%s\n", info.env[i]);
// 	// 	fflush(stdout);
// 	// 	i++;
// 	// }
// }
