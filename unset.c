/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:36:35 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/08 17:01:09 by jormoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int run_unset(t_mini *mini, t_command *cmd)
{
	unset_export(mini, cmd);
	unset_envp(mini, cmd);
	return(0);
}

void unset_envp(t_mini *mini, t_command *cmd)
{
	char **temp;
	int	i;
	int j;
	int t;

	t = 0;
	i = 0;
	while(mini->envp[i])
		i++;
	temp = ft_calloc((i + 1),(sizeof(char *)));
	i = 0;
	while(mini->envp[i])
	{
		j = 1;
		while(cmd->arg_array[j] && !ft_strchr(cmd->arg_array[j], '=') )
		{
			if(!ft_strncmp(mini->envp[i], cmd->arg_array[j], 
				ft_len_before_equal(mini->envp[i])))
			{
				free(mini->envp[i]);
				mini->envp[i] = NULL;
				break ;
			}
			j++;
		}
		if(!cmd->arg_array[j])
			temp[t++] = mini->envp[i];
		i++;
	}
	free(mini->envp);
	mini->envp = temp;
}


void	unset_export(t_mini *mini, t_command *cmd)
{
	int	i;
	int j;
	int t;
	char ***temp;

	t = 0;
	i = 0;
	while(mini->envp_dictionary[i])
		i++;
	temp = ft_calloc((i + 1),(sizeof(char **)));
	i = 0;
	while(mini->envp_dictionary[i] && mini->envp_dictionary[i][0])
	{
		j = 1;
		while(cmd->arg_array[j] && !ft_strchr(cmd->arg_array[j], '='))
		{
			if(!ft_strncmp(mini->envp_dictionary[i][0], cmd->arg_array[j], 
				ft_strlen(mini->envp_dictionary[i][0])))
			{
				free_split(mini->envp_dictionary[i]);
				mini->envp_dictionary[i] = NULL;
				break ;
			}
			j++;
		}
		if(!cmd->arg_array[j])
			temp[t++] = mini->envp_dictionary[i];
		i++;
	}
	free(mini->envp_dictionary);
	mini->envp_dictionary = temp;
}
