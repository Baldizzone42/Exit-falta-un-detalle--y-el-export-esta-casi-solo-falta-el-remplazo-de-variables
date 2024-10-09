# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jormoral <jormoral@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 12:03:18 by labderra          #+#    #+#              #
#    Updated: 2024/10/08 17:38:45 by jormoral         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = clang -Wall -Werror -Wextra -g -fsanitize=address
LIBFT = libft/libft.a
SRC = main.c \
lexer.c \
parser.c \
exec.c \
tools.c \
environment.c \
builtin.c \
unset.c \
cd.c \

OBJ = $(SRC:.c=.o)

all : $(NAME)

%.o : %.c 
	$(CC) -c $< -o $@

$(LIBFT):
	make -C libft 
	make -C libft bonus

$(NAME) : $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -lreadline -o $@

clean :
	rm $(OBJ)
	make -C libft clean

fclean : clean
	rm $(NAME)
	make -C libft fclean

re : fclean all

test : all clean
	./minishell

.PHONY : all clean fclean re test