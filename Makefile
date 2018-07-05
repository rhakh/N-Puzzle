CXX=g++

NAME = n-puzzle

FLAGS =  -Wall -Wextra -Werror -std=c++11 -Wno-unused -lboost_filesystem  -lboost_system  -pthread -lboost_thread

OBJDIR = obj/

SRCDIR = src/

_SRC = heuristicFunctions.cpp \
		NPuzzleSolver.cpp \
		State.cpp \
		main.cpp \

SRC = $(addprefix $(SRCDIR), $(_SRC))

OBJ = $(addprefix $(OBJDIR),$(_SRC:.cpp=.o))

all: make_dir $(NAME)

make_dir:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(FLAGS) -o $@ -c $<

$(NAME): $(OBJ) $(SRC)
	$(CXX) $(FLAGS) -lncurses -o $(NAME) $(OBJ)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
