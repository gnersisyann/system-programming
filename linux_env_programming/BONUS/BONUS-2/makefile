NAME = calc
FLAGS = -Wall -Wextra
SRC = main.cpp
OBJ = main.o
GCC = g++

all: $(NAME)

$(NAME) : $(OBJ)
	@ $(GCC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJ) : $(SRC)
	@ $(GCC) $(FLAGS) -c $(SRC) -o $(OBJ)

clean:
	@ rm -rf $(OBJ) $(NAME) out_*

fclean:
	@ rm -f $(OBJ)
