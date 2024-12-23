NAME1 = check
NAME2 = destroy
NAME3 = init
NAME4 = set
FLAGS = -O3
SRC1 = check.cpp
SRC2 = destroy.cpp
SRC3 = init.cpp
SRC4 = set.cpp
OBJ1 = check.o
OBJ2 = destroy.o
OBJ3 = init.o
OBJ4 = set.o
GCC = g++

all: $(NAME1) $(NAME2) $(NAME3) $(NAME4)

$(NAME1): $(OBJ1)
	@$(GCC) $(FLAGS) $(OBJ1) -o $(NAME1)

$(NAME2): $(OBJ2)
	@$(GCC) $(FLAGS) $(OBJ2) -o $(NAME2)

$(NAME3): $(OBJ3)
	@$(GCC) $(FLAGS) $(OBJ3) -o $(NAME3)

$(NAME4): $(OBJ4)
	@$(GCC) $(FLAGS) $(OBJ4) -o $(NAME4)

$(OBJ1): $(SRC1)
	@$(GCC) $(FLAGS) -c $(SRC1) -o $(OBJ1)

$(OBJ2): $(SRC2)
	@$(GCC) $(FLAGS) -c $(SRC2) -o $(OBJ2)

$(OBJ3): $(SRC3)
	@$(GCC) $(FLAGS) -c $(SRC3) -o $(OBJ3)

$(OBJ4): $(SRC4)
	@$(GCC) $(FLAGS) -c $(SRC4) -o $(OBJ4)

clean:
	@rm -rf $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(NAME1) $(NAME2) $(NAME3) $(NAME4)

fclean:
	@rm -f $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)

re: fclean all
