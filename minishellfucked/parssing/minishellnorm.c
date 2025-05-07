/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellnorm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkurukul <thilinaetoro4575@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:22:23 by ilmahjou          #+#    #+#             */
/*   Updated: 2025/05/07 23:46:16 by tkurukul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../built-ins/minishell.h"

static t_token	*handle_pipe(int *i, t_token *head, t_token *current)
{
	t_token	*new_token;

	new_token = creat_token("|", TOKEN_PIPE);
	if (!new_token)
		return (free_tokens(head));
	if (!head)
		head = new_token;
	else
		current->next = new_token;
	(*i)++;
	return (head);
}

static t_token	*handle_redirection(char *input, int *i, t_token *head, t_token *current)
{
	t_token	*new_token;

	new_token = NULL;
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			new_token = creat_token("<<", TOKEN_HEREDOC);
			*i += 2;
		}
		else
		{
			new_token = creat_token("<", TOKEN_REDIR_IN);
			(*i)++;
		}
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			new_token = creat_token(">>", TOKEN_REDIR_APPEND);
			*i += 2;
		}
		else
		{
			new_token = creat_token(">", TOKEN_REDIR_OUT);
			(*i)++;
		}
	}
	if (!new_token)
		return (free_tokens(head));
	if (!head)
		head = new_token;
	else
		current->next = new_token;
	return (head);
}

// New function to join adjacent segments into one word
static t_token *join_word_segment(char *segment, t_token *head, t_token **current_word_token)
{
	char *new_content;
	t_token *new_token;

	if (!segment)
		return (free_tokens(head));

	if (*current_word_token == NULL)
	{
		// No current word being built, create a new token
		new_token = creat_token(segment, TOKEN_WORD);
		free(segment);
		if (!new_token)
			return (free_tokens(head));

		if (!head)
			head = new_token;
		else
			get_last_token(head)->next = new_token;

		*current_word_token = new_token;
	}
	else
	{
		// Append to existing word token
		new_content = ft_strjoin((*current_word_token)->content, segment);
		free(segment);
		if (!new_content)
			return (free_tokens(head));

		free((*current_word_token)->content);
		(*current_word_token)->content = new_content;
	}

	return (head);
}

// Modified handle_word to extract word segment
static char *extract_word_segment(char *input, int *i)
{
	int start;
	//char *word;

	start = *i;
	while (input[*i] && !ft_strchr(" \t|><\'\"$", input[*i]))
		(*i)++;

	if (*i > start)
		return ft_substr(input, start, *i - start);
	return NULL;
}

// Modified handle_single_quotes to return string content
static char *extract_single_quote_content(char *input, int *i)
{
	int content_start = *i + 1; // Skip the opening quote
	char quote_char = input[*i];
	char *content;

	(*i)++; // Skip the opening quote

	// Find the closing quote
	while (input[*i] && input[*i] != quote_char)
		(*i)++;

	if (!input[*i]) // No closing quote found
	{
		printf("error: unclosed quote\n");
		return NULL;
	}

	content = ft_substr(input, content_start, *i - content_start);
	(*i)++; // Skip the closing quote
	return content;
}

// Modified handle_double_quotes to return string content
static char *extract_double_quote_content(char *input, int *i)
{
	int content_start = *i + 1; // Skip the opening quote
	char quote_char = input[*i];
	char *content;

	(*i)++; // Skip the opening quote
	// Find the closing quote
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!input[*i]) // No closing quote found
	{
		printf("error: unclosed quote\n");
		return NULL;
	}
	content = ft_substr(input, content_start, *i - content_start);
	(*i)++; // Skip the closing quote
	return content;
}

// Modified tokenize_input to handle adjacent quoted segments
t_token *tokenize_input(char *input)
{
	t_token	*head = NULL;
	t_token	*current_word_token = NULL;
	int		i = 0;
	char	*segment = NULL;

	while (input[i])
	{
		if (input[i] == ' ' || input[i] == '\t') // Space - end of current word
		{
			current_word_token = NULL; // Reset current word
			i++;
		}
		else if (input[i] == '|') // Pipe - end of current word and command
		{
			current_word_token = NULL; // Reset current word
			head = handle_pipe(&i, head, get_last_token(head));
		}
		else if (input[i] == '<' || input[i] == '>') // Redirection - end of current word
		{
			current_word_token = NULL; // Reset current word
			head = handle_redirection(input, &i, head, get_last_token(head));
		}
		else if (input[i] == '\'') // Single quotes
		{
			segment = extract_single_quote_content(input, &i);
			if (!segment)
				return (free_tokens(head));
			head = join_word_segment(segment, head, &current_word_token);
		}
		else if (input[i] == '"') // Double quotes
		{
			segment = extract_double_quote_content(input, &i);
			if (!segment)
				return (free_tokens(head));
			head = join_word_segment(segment, head, &current_word_token);
		}
		else if (input[i] == '$') // Variable
		{
			// For now, handle variables as separators (we'll need to modify this later)
			current_word_token = NULL;
			head = handle_env_variable(input, &i, head, get_last_token(head));
		}
		else // Regular word character
		{
			segment = extract_word_segment(input, &i);
			if (segment) // Only process if we got something
				head = join_word_segment(segment, head, &current_word_token);
		}
		if (!head)
			return (NULL);  // If error occurred, return NULL
	}
	return (head);  // Return the list of tokens
}

// The rest of minishellnorm.c remains unchanged
// int main(void)
// {
// 	char *line;
// 	t_token *tokens;
// 	t_token *current;
// 	t_info info;

// 	ft_printf(RED BOLD);
// 	ft_printf(" __  __ _____ _   _ _____ _____ _   _ _____ _      _     \n");
// 	ft_printf("|  \\/  |_   _| \\ | |_   _|/ ____| | | | ____| |    | |    \n");
// 	ft_printf("| \\  / | | | |  \\| | | | | (___ | |_| |  _| | |    | |    \n");
// 	ft_printf("| |\\/| | | | | . ` | | |  \\___ \\|  _  | |___| |    | |    \n");
// 	ft_printf("| |  | |_| |_| |\\  |_| |_ ____) | | | |_____|_|____|_|____\n");
// 	ft_printf("|_|  |_|_____|_| \\_|_____|_____/|_| |_|______|______|______|\n");
// 	ft_printf(RESET);
// 	ft_printf("\n     MINISHELL by Ilyas & Thilina - Core Dumpers 💥\n\n");
// 	// Initialize environment
// 	form_env(__environ, &info);
// 	using_history();
// 	while (1)
// 	{
// 		line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
// 		if (!line)
// 			break;
// 		if (line[0] != '\0')
// 		{
// 			add_history(line);
// 			tokens = tokenize_input(line);
// 			// Debug: Print tokens
// 			current = tokens;
// 			printf("Tokens:\n");
// 			while (current)
// 			{
// 				printf("Token: %s, Type: %d\n", current->content, current->type);
// 				current = current->next;
// 			}
// 			if (tokens) {
// 				t_command *commands = tokens_to_commands(tokens);
// 				// Debug: Print commands
// 				t_command *cmd_debug = commands;
// 				printf("Commands:\n");
// 				while (cmd_debug) {
// 					printf("Command: ");
// 					if (cmd_debug->args) {
// 						for (int i = 0; cmd_debug->args[i]; i++) {
// 							printf("%s ", cmd_debug->args[i]);
// 						}
// 					} else {
// 						printf("(no args)");
// 					}
// 					printf("\n");
// 					cmd_debug = cmd_debug->next;
// 				}
// 				execute_commands(commands, &info);
// 				free_commands(commands);
// 				free_tokens(tokens);
// 			}
// 		}
// 		free(line);
// 	}
// 	clear_history();
// 	// Clean up environment
// 	free_mat(info.env);
// 	return 0;
// }

int main(int ac, char **av, char **env)
{
    t_token     *token;
    t_info      info;
    char        *line;
	int i;
	int j;

    if (ac != 1 || av[1])
    {
        write(2, "Error: Invalid arguments\n", 24);
        return (1);
    }
    form_env(env, &info);
    using_history();
    while (1)
    {
        info.pos = 0;
        info.flag_ri = 0;
        info.exec = NULL;
        info.env = env;
        info.tmp = NULL;
        //signal(SIGINT, ctrl_c);
        signal(SIGQUIT, estat);
        line = readline("\001\033[1;32m\002minishell$ \001\033[0m\002");
        if (!line)
        {
            write(1, "exit\n", 5);  // Print exit message on Ctrl+D
            break;
        }
        if (line[0] != '\0')
        {
            add_history(line);
            token = tokenize_input(line);  // Tokenize the input line
            if (token)
            {
				printf("%d\n", token->type);
                form_main(token, &info);
				i = 0;
				while (info.exec[i])
				{
					j = 0;
					arg_execve(&info.exec[i], &info);
					while (info.exec[i][j])
					{
						printf("%s\n", info.exec[i][j]);
						j++;
					}
					printf("----------------------\n");
					i++;
					fflush(stdout);
				}
                ft_execution(&info);
                free_tokens(token);  // Use your token free function
            }
        }
        free(line);  // Free the line after processing
		if (info.exec && *info.exec)  // Check if it exists and points to something
    		free_mat(*info.exec);  // Pass the char** that the char*** points to
    }
    clear_history();
    return (0);
}
