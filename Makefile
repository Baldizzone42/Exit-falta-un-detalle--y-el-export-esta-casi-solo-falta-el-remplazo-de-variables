# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: labderra <labderra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 12:03:18 by labderra          #+#    #+#              #
#    Updated: 2024/09/30 19:45:15 by labderra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = clang -Wall -Werror -Wextra -g #-fsanitize=address
LIBFT = libft/libft.a
SRC = main.c \
lexer.c \
parser.c \
exec.c \
tools.c \
environment.c \
builtin.c

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