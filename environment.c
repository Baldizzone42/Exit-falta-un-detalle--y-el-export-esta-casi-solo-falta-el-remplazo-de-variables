/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: labderra <labderra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/10/02 10:12:35 by labderra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_full_path(char **envp)
{
	while (envp && *envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!envp || !*envp)
		return (ft_split("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:\
			/sbin:/bin:.", ':'));
	return (add_str_to_array(".", ft_split(*envp + 5, ':')));
}

static char	*get_env_item(char **envp, char *item)
{
	char	*search;
	
	search = ft_strjoin(item, "=");
	if (!search)
		return (item);
	while (envp && *envp && ft_strncmp(*envp, search, ft_strlen(search)))
		envp++;
	free(search);
	if (!envp || !*envp)
		return (ft_strdup(""));
	return (ft_strdup(*envp + ft_strlen(item) + 1));
}

char	**copy_environmet(char **envp)
{
	char	**p;
	int		i;

	i = 0;
	p = ft_calloc(sizeof(char *), 1);
	while (envp && envp[i])
		p = add_str_to_array(envp[i++], p);		
	return (p);
}

char	***copy_split_environment(char **envp)
{
	char	***p;
	int		i;
	
	i = 0;
	while(envp && envp[i])
		i++;
	p = ft_calloc(sizeof(char **), i + 1);
	i = 0;
	while (envp && envp[i])
	{
		p[i] = ft_split(envp[i], '=');
		i++;
	}	
	return (p);
}

void	insert_variable_value(t_mini *mini, char **str)
{
	char	*identifier;
	char	*value;
	int		j;

	if (**str == '0' && ++*str)
		value = ft_strdup(mini->argv[0]);
	else if (**str > '0' && **str <= '9' && ++*str)
		value = ft_strdup("");
	else
	{
		j = 0;
		identifier = ft_calloc(sizeof(char), ft_strlen(*str));
		while (**str == '_' || ft_isalnum(**str))
			identifier[j++] = *(*str)++;
		value = get_env_item(mini->envp, identifier);
		free(identifier);
	}
	*str = join_before_line(value, *str);
}
