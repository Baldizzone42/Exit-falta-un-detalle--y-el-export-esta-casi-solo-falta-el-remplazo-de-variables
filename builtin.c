/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:42:34 by jormoral          #+#    #+#             */
/*   Updated: 2024/10/05 19:35:32 by jormoral         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pwd(t_mini *mini, t_command *cmd)
{
	char	*pwd;

	apply_redir(cmd);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	else
	{
		printf("%s\n", pwd);
		free (pwd);
		revert_redir(mini, cmd);
		return (0);
	}
}

int	run_echo(t_mini *mini, t_command *cmd)
{
	int	nflag;
	int	i;

	apply_redir(cmd);
	i = 0;
	nflag = 0;
	if (cmd->arg_array[1] && cmd->arg_array[1][i++] == '-')
	{
		while (cmd->arg_array[1][i] == 'n')
			i += 1;
		nflag = cmd->arg_array[1][i] == '\0';
	}
	i = 1;
	while (cmd->arg_array[i + nflag])
	{
		printf("%s", cmd->arg_array[i + nflag]);
		if (cmd->arg_array[++i + nflag])
			printf(" ");
	}
	if (nflag == 0)
		printf("\n");
	revert_redir(mini, cmd);
	return (0);
}

int	run_env(t_mini *mini, t_command *cmd)
{
	char	**aux;

	apply_redir(cmd);
	aux = mini->envp;
	if (!aux)
	{
		revert_redir(mini, cmd);
		return (1);
	}
	while (aux && *aux)
		printf("%s\n", *aux++);
	revert_redir(mini, cmd);
	return (0);
}

int	run_exit(t_mini *mini, t_command *cmd)
{
	unsigned long long	result;
	char				*temp;
	(void)				mini;
	int					flaki;
	int					i;

	i = 0;
	flaki = 0;
	result = 0;
	while(cmd->arg_array[1] && cmd->arg_array[1][i])
	{
		if((cmd->arg_array[1][i] < '0' || cmd->arg_array[1][i] > '9'))
		{
			printf("exit\n");
			printf("bash: exit: %s : numeric argument required\n", cmd->arg_array[1]);
			exit(2);
		}
		i++;
	}
	if(cmd->arg_array[0] && cmd->arg_array[1] && cmd->arg_array[2])
	{
		printf("exit\n");
		printf("bash: exit: : too many arguments\n");
		exit(1);
	}
	if (cmd->arg_array && !cmd->arg_array[1])
		exit (0);
	if (!cmd->arg_array[1][0]) 
	{
		printf("exit\n");
		printf("bash: exit: : numeric argument required\n");
		exit(2);
	}
	if (cmd->arg_array[1][i] == '-')
	{
		flaki = 1;
		i++;
	}
	temp = ft_strdup(cmd->arg_array[1] + flaki);
	if (cmd->arg_array && cmd->arg_array[1])
		result = ft_atoll(temp);
	if (result > 9223372036854775807)
	{
		printf("exit\n");
		printf("bash: exit: %s : numeric argument required\n", cmd->arg_array[1]);
		exit(2);
	}
	flaki = 1;
	if(cmd->arg_array[1][0] == '-')
		flaki = -1;
	printf("exit\n");
	exit((result * flaki) % 256);
}

int	check_export(char *str, int *flag)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
		i++;
	else
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
		i++;
	}
	if (str[i] == '=')
		*flag = 1;
	return (1);
}

int	search_env(t_mini *mini, char *str)
{
	int	i;

	i = 0;
	while (mini->envp[i])
	{
		if (!ft_strncmp(mini->envp[i], str, ft_strlen(str)))
			break ;
		i++;
	}
	return (i);
}

void ok_replace(t_mini *mini, t_command *cmd, int i, int na)
{
	int		i_env;

	i_env = 0;
	free(mini->envp_dictionary[i][1]);
	mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
	i_env = search_env(mini, mini->envp_dictionary[i][0]);
	if (!mini->envp[i_env])
		mini->envp = add_str_to_array(cmd->arg_array[na], mini->envp);
	else
	{
		free(mini->envp[i_env]);
		mini->envp[i_env] = ft_strdup(cmd->arg_array[na]);
	}
}

void ko_replace(t_mini *mini, t_command *cmd, int i, int na)
{
	mini->envp_dictionary = triple_copy_add(mini->envp_dictionary);
	mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
	mini->envp = add_str_to_array(cmd->arg_array[na], mini->envp);
}

void replace_export(t_mini *mini, t_command *cmd, int na, int flag)
{
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (cmd->arg_array[na][len] && cmd->arg_array[na][len] != '=')
		len++;
	while (mini->envp_dictionary[i])
	{
		if (!ft_strncmp(mini->envp_dictionary[i][0], cmd->arg_array[na], len))
			break;
		i++;
	}
	if(mini->envp_dictionary[i] && flag)
		ok_replace(mini, cmd, i, na);
	else
	{
		if (flag)
			ko_replace(mini, cmd, i, na);
		else
		{
			mini->envp_dictionary = triple_copy_add(mini->envp_dictionary);
			mini->envp_dictionary[i] = ft_split(cmd->arg_array[na], '=');
		}
	}
}

int	only_export(t_mini *mini, t_command *cmd)
{
	int		i;
	char	***env;

	i = 0;
	apply_redir(cmd);
	env = mini->envp_dictionary;
	alpha_export(env);
	while (env[i])
	{
		if (!env[i][1])
			printf("declare -x %s\n", env[i][0]);
		else
			printf("declare -x %s=\"%s\"\n", env[i][0], env[i][1]);
		i++;
	}
	revert_redir (mini, cmd);
	return (0);
}

void	alpha_export(char ***env)
{
	int		c;
	int		i;
	char	**temp;

	i = 0;
	while (env[i] && env[i + 1])
	{
		c = 0;
		while (env[i][0][c] == env[i + 1][0][c])
			c++;
		if (env[i][0][c] > env[i + 1][0][c])
		{
			temp = env[i];
			env[i] = env[i + 1];
			env[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int	run_export(t_mini *mini, t_command *cmd)
{
	int		i;
	int		c;
	int		flag;
	char	**temp;

	temp = NULL;
	flag = 0;
	c = 1;
	i = 1;
	if (cmd->arg_array && !cmd->arg_array[1])
		cmd->exit_status = only_export(mini, cmd);
	while (cmd->arg_array[i])
	{
		if (!check_export(cmd->arg_array[i], &flag))
		{
			printf("bash: export:");
			printf(" ´%s': not a valid identifier\n", cmd->arg_array[i]);
			cmd->exit_status = 1;
		}
		else
			replace_export(mini, cmd, i, flag);
		i++;
	}
	return (cmd->exit_status);
}
