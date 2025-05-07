/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 19:55:30 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/07 20:12:21 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../built-ins/minishell.h"

t_token *creat_token(char *content, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return NULL;
	new_token->content = ft_strdup(content);
	if (!new_token->content)
	{
		free(new_token);
		return NULL;
	}
	new_token->type = type;
	new_token->next = NULL;
	return new_token;
}
t_token *free_tokens(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free (tmp->content);
		free(tmp);
	}
	return NULL;
}

t_token	*get_last_token(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

t_token *handle_env_variable(char *input, int *i, t_token *head, t_token *current)
{
	int     start;
	char    *var_name;
	t_token *new_token;

	(*i)++; // Skip the $

	// Handle special case for $? (last exit status)
	if (input[*i] == '?')
	{
		var_name = ft_strdup("?");
		if (!var_name)
			return (free_tokens(head));
		(*i)++;
		new_token = creat_token(var_name, TOKEN_VAR);  // Create a TOKEN_VAR for the special variable
		free(var_name);
	}
	else
	{
		// Parse the variable name
		start = *i;
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;

		if (*i == start)
			return (head); // No valid variable, just return
		var_name = ft_substr(input, start, *i - start);
		if (!var_name)
			return (free_tokens(head));
		// Create a token for the variable (e.g., HOME, USER)
		new_token = creat_token(var_name, TOKEN_VAR);
		free(var_name);
	}
	// Add the new token to the token list
	if (!new_token)
		return (free_tokens(head));

	if (!head)
		head = new_token;
	else
		current->next = new_token;
	return (head);
}

// void free_commands(t_command *head)
// {
// 	t_command *tmp;

// 	while (head) {
// 		tmp = head;
// 		head = head->next;

// 		// Free arguments
// 		if (tmp->args) {
// 			for (int i = 0; tmp->args[i]; i++)
// 				free(tmp->args[i]);
// 			free(tmp->args);
// 		}

// 		// Free redirections (to be implemented)

// 		free(tmp);
// 	}
// }
