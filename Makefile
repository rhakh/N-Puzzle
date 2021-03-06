NAME = npuzzle

OS := $(shell uname)
ifeq ($(OS),Darwin)
  CXX=clang++
  INCLUDE_AND_LIBS = -I /usr/local/Cellar/boost/1.72.0_3/include \
					-L /usr/local/Cellar/boost/1.72.0_3/lib \
					-I Simple-Web-Server
  FLAGS = -std=c++14 -Wall -Wextra -Werror \
			-lboost_filesystem  -lboost_system  -lboost_program_options \
			-pthread -lboost_thread-mt -Wno-unused-command-line-argument \
			-Wno-unused -Wno-unused-parameter -O2
else
  CXX=g++
  INCLUDE_AND_LIBS = -I Simple-Web-Server -L /usr/lib/x86_64-linux-gnu -I /usr/include/boost
  FLAGS = -std=c++11 -Wall -Wextra -Werror \
			-lboost_system -lboost_filesystem -lboost_program_options \
			-lpthread -lboost_thread -Wno-unused-command-line-argument \
			-Wno-unused -Wno-unused-parameter -O2
endif


OBJDIR = obj/

SRCDIR = src/

_SRC = 							\
		State.cpp \
		NPuzzleSolver.cpp \
		main.cpp \
		CSCP.cpp \
		Heuristic.cpp \
		CLI.cpp \

SRC = $(addprefix $(SRCDIR), $(_SRC))

OBJ = $(addprefix $(OBJDIR),$(_SRC:.cpp=.o))

all: make_dir $(NAME)

make_dir:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(INCLUDE_AND_LIBS) $(FLAGS) -o $@ -c $<

$(NAME): $(OBJ) $(SRC)
	$(CXX) $(INCLUDE_AND_LIBS) -o $(NAME) $(OBJ) $(FLAGS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
