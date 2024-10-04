/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:28:37 by labderra          #+#    #+#             */
/*   Updated: 2024/10/03 11:46:50 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*new_command(void)
{
	t_command	*cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->arg_array = ft_calloc(sizeof(char *), 1);
	if (!cmd->arg_array)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->path = NULL;
	cmd->infile = -1;
	cmd->outfile = -1;
	cmd->pid = -1;
	cmd->exit_status = -1;
	cmd->next = NULL;
	return (cmd);
}

static int heredoc(char *lmt, int xpand)
{
	char	*aux_str;
	int		tmp_file_fd;
	int		size;
	(void)xpand;

	size = ft_strlen(lmt);
	tmp_file_fd = open(".heredoctmp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (tmp_file_fd == -1)
		return (-1);
	write(1, "heredoc>", 9);
	aux_str = get_next_line(0);
	while (aux_str)
	{
		if (!ft_strncmp(aux_str, lmt, size) && aux_str[size] == '\n')
		{
			free(aux_str);
			break ;
		}
		write(tmp_file_fd, aux_str, ft_strlen(aux_str));
		free(aux_str);
		write(1, "heredoc>", 9);
		aux_str = get_next_line(0);
	}
	close(tmp_file_fd);
	return (open(".heredoctmp", O_RDONLY));
}

static void	handle_redir(t_command *cmd, char *redir, char *filename)
{
	int	file_fd;
	
	if (!ft_strncmp(redir, "<<", 3))
		file_fd = heredoc(filename, 1);
	else if (!ft_strncmp(redir, "<$", 3))
		file_fd = heredoc(filename, 0);
	else if (!ft_strncmp(redir, "<", 2))
		file_fd = open(filename, O_RDONLY);
	else if (!ft_strncmp(redir, ">>", 3))
		file_fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
	else
		file_fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (file_fd == -1)
		perror("file error");
	if (*redir == '<')
		cmd->infile = file_fd;
	else
		cmd->outfile = file_fd;
}

static void	handle_name(char *name, t_command *cmd)
{
	cmd->arg_array =  add_str_to_array(name, cmd->arg_array);
}

static void	insert_command(t_mini *mini, t_command *cmd)
{
	t_command	*p;

	if (!mini->cmd_list)
		mini->cmd_list = cmd;
	else
	{
		p = mini->cmd_list;
		while (p->next)
			p = p->next;
		p->next = cmd;
	}	
}

void	parser(t_mini *mini)
{
	t_command	*cmd;
	t_tkn		*p;

	p = mini->tkn_list;
	while (p)
	{
		cmd = new_command();
		while (p && p->tkn_type != 0)
		{
			if (p->tkn_type == 1)
			{
				handle_redir(cmd, p->tkn, p->next->tkn);
				p = p->next;
			}
			else if (p->tkn_type == 2)
				handle_name(p->tkn, cmd);
			p = p->next;
		}
		if (p && p->tkn_type == 0)
		{
			insert_command(mini, cmd);
			cmd = new_command();
			p = p->next;
		}
		else
			insert_command(mini, cmd);
	}
}