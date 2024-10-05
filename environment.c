/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 11:11:44 by labderra          #+#    #+#             */
/*   Updated: 2024/10/05 17:46:22 by jormoral         ###   ########.fr       */
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

char ***triple_copy_add(char ***triple)
{
	char ***result;
	int 	i;

	i = 0;
	while(triple[i])
		i++;
	result = ft_calloc(sizeof(char **), i + 2);
	i = 0;
	while(triple[i])
	{
		result[i] = copy_environmet(triple[i]);
		i++;
	}
	free_dictionary(triple);
	return(result);
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
