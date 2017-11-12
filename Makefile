# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhakh <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/28 15:15:42 by rhakh             #+#    #+#              #
#    Updated: 2017/09/24 13:30:36 by rhakh            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = npuzzle

OS = $(shell uname -s)

SRC =   main.cpp \

SRCDIR = ./src/

OBJDIR = ./obj/

OBJ = $(addprefix $(OBJDIR),$(SRC:.cpp=.o))

ifeq ($(OS),Linux)
    CFLAGS += -Wall -Wextra -Werror `pkg-config --cflags gtk+-3.0`
    LFLAGS += `pkg-config --libs gtk+-3.0`
endif

ifeq ($(OS),Darwin)
    CFLAGS += -Wall -Wextra -Werror sdfdsf
    LFLAGS += afaf
endif


all: obj $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.cpp
	g++ $(CFLAGS) -o $@ -c $< $(LFLAGS)

$(NAME): $(OBJ)
	g++ $(CFLAGS) -o $(NAME) $(OBJ) $(LFLAGS)

clean:
	/bin/rm -rf $(OBJDIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
