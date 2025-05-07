/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilmahjou <ilmahjou@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 17:37:18 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/07 19:59:10 by ilmahjou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../built-ins/minishell.h"
// Helper for single quotes
t_token *handle_single_quotes(char *input, int *i, t_token *head, t_token *current)
{
	int		content_start = *i + 1; // Skip the opening quote
	char	quote_char = input[*i];
	char	*word;
	t_token	*new_token;

	(*i)++; // Skip the opening quote

	// Find the closing quote
	while (input[*i] && input[*i] != quote_char)
		(*i)++;

	if (!input[*i]) // No closing quote found
		return (printf("error\n"), free_tokens(head)); // Error: unclosed quote

	word = ft_substr(input, content_start, *i - content_start);
	if (!word)
		return (free_tokens(head));

	new_token = creat_token(word, TOKEN_SQUOTE);
	free(word);

	if (!new_token)
		return (free_tokens(head));

	if (!head)
		head = new_token;
	else
		current->next = new_token;

	(*i)++; // Skip the closing quote
	return (head);
}

// Helper to add text in double quotes
t_token *add_double_quote_text(char *input, int start, int end,
								t_token *head, t_token *current)
{
	char    *word;
	t_token *new_token;

	word = ft_substr(input, start, end - start);
	if (!word)
		return (free_tokens(head));

	new_token = creat_token(word, TOKEN_DQUOTE);
	free(word);

	if (!new_token)
		return (free_tokens(head));

	if (!head)
		head = new_token;
	else
		current->next = new_token;

	return (head);
}

// Process content inside double quotes
t_token *process_double_quotes(char *input, int *i, t_token *head, t_token *current)
{
	char    quote_char = input[(*i) - 1]; // Quote char is the previous char
	int     content_start = *i;
	while (input[*i] && input[*i] != quote_char)
	{
		if (input[*i] == '$' &&
			(ft_isalnum(input[*i + 1]) || input[*i + 1] == '_' || input[*i + 1] == '?'))
		{
			if (*i > content_start)
			{
				head = add_double_quote_text(input, content_start, *i, head, current);
				if (!head)
					return (NULL);
				current = get_last_token(head);
			}
			head = handle_env_variable(input, i, head, current);
			if (!head)
				return (NULL);

			current = get_last_token(head);
			content_start = *i; // Reset content start position
		}
		else
			(*i)++;
	}
	if (*i > content_start)
	{
		head = add_double_quote_text(input, content_start, *i, head, current);
		if (!head)
			return (NULL);
	}
	if (!input[*i]) // No closing quote found
		return (printf("error\n"), free_tokens(head)); // Error: unclosed quote
	(*i)++; // Skip the closing quote
	return (head);
}

// Main quotes handler function
t_token *handle_quotes(char *input, int *i, t_token *head, t_token *current)
{
	char quote_char = input[*i];

	if (quote_char == '\'')
		return (handle_single_quotes(input, i, head, current));
	else // Double quotes
	{
		(*i)++; // Skip the opening quote
		return (process_double_quotes(input, i, head, current));
	}
}
