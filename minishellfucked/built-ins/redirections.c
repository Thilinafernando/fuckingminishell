/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <tkurukul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 22:04:01 by tkurukul          #+#    #+#             */
/*   Updated: 2025/05/08 15:10:21 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// if open fails, estat to the right number

#include "minishell.h"

int	ft_input(char **exec)
{
	int	fd;

	fd = open(exec[1], O_RDONLY);
	if (fd == -1)
		return (estat(126), -1);
	if(dup2(fd, 0) == -1)
	{
		ft_printf(2, "Minishell: failed dup\n");
		return (estat(1), -1);
	}
	close(fd);
	return (0);
}

int	ft_output(char **exec)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT| O_TRUNC, 0644);
	if (fd == -1)
		return (estat(1), -1);
	if(dup2(fd, 1) == -1)
	{
		ft_printf(2, "Minishell: failed dup\n");
		return (estat(1), -1);
	}
	close(fd);
	return (0);
}

int	ft_append(char **exec)
{
	int	fd;

	fd = open(exec[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
	if(dup2(fd, 1) == -1)
	{
		ft_printf(2, "Minishell: failed dup\n");
		return (-1);
	}
	close(fd);
	return (0);
}

void	ft_heredoc_process(char **exec, int pipefd[2])
{
	char	*str;
	int		i;

	signal(SIGQUIT, SIG_DFL);
	while (1)
	{
		str = readline("> ");
		if (!str || !exec[1] || ft_strcmp(str, exec[1]) == 0)
		{
			free(str);
			ft_printf(1, "\n");
			close(pipefd[1]);
			exit(0);
		}
		i = ft_strlen(str);
		ft_printf(pipefd[1],"%s\n", str);
		free(str);
	}
}

int	ft_heredoc_parent(int pid, int pipeh[2], int *status)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
	set_signals();
	if (WIFSIGNALED(*status))
	{
		close(pipeh[0]);
		if (WTERMSIG(*status) == SIGINT)
			return (-1);
		if (WTERMSIG(*status) == SIGQUIT)
		{
			ft_printf(1, "Quit (core dumped)\n");
			estat(131);
			return (-1);
		}
	}
	// this next if converts the standard in into what thier pipe had read
	if (dup2(pipeh[0], STDIN_FILENO) == -1)
		return (estat(1), -1);
	close(pipeh[0]);
	return (estat(0), 0);
}

int		ft_heredoc(char **exec)
{
	int		status;
	int		pipeh[2];
	pid_t	pid;

	if (pipe(pipeh) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipeh[0]);
		signal(SIGINT, SIG_DFL);
		ft_heredoc_process(exec, pipeh);
		exit(0);
	}
	else
	{
		close(pipeh[1]);
		if (ft_heredoc_parent(pid, pipeh, &status) == -1)
			return (-1);
	}
	return (0);
}
