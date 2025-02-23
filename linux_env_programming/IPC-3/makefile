NAME1 = first
NAME2 = second
FLAGS = -O3
SRC1 = first.cpp
SRC2 = second.cpp
OBJ1 = first.o
OBJ2 = second.o
GCC = g++

all: $(NAME1) $(NAME2)

$(NAME1): $(OBJ1)
	@$(GCC) $(FLAGS) $(OBJ1) -o $(NAME1)

$(NAME2): $(OBJ2)
	@$(GCC) $(FLAGS) $(OBJ2) -o $(NAME2)

$(OBJ1): $(SRC1)
	@$(GCC) $(FLAGS) -c $(SRC1) -o $(OBJ1)

$(OBJ2): $(SRC2)
	@$(GCC) $(FLAGS) -c $(SRC2) -o $(OBJ2)

clean:
	@rm -rf $(OBJ1) $(OBJ2) $(NAME1) $(NAME2)

fclean:
	@rm -f $(OBJ1) $(OBJ2)

re: fclean all
