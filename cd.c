/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:38:19 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/08 21:11:14 by jormoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_envp(t_mini *mini, t_command *cmd)
{
	char *path;
	path = getcwd(NULL, 0);
	ok_replace(mini, cmd, )
	
	
}


int run_cd(t_mini *mini, t_command *cmd)
{
	int	result;
	int	i;
	(void)mini;

	i = 0;
	result = 0;
	if(cmd->arg_array && (!cmd->arg_array[1] || cmd->arg_array[1][0] == '~'))
	{
		i = search_env(mini, "HOME=");
		if(chdir(mini->envp[i] + 5) == -1)
			perror("HOME NOT SET");
	}
	else if(cmd->arg_array && cmd->arg_array[1][0] == '-')
	{
		i = search_env(mini, "OLDPWD=");
		if(chdir(mini->envp[i] + 7) == -1)
			perror("OLDPWD NOT SET");
	}
	else
	{
		if(chdir(cmd->arg_array[1]) == -1)
			perror("NOT FOUND");
		
	}
	
	return(result);
}