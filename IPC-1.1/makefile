NAME = program
NAME1 = killer
NAME2 = victim
FLAGS = -O3 -Wall
SRC1 = killer.cpp
SRC2 = victim.cpp
OBJ1 = killer.o
OBJ2 = victim.o
GCC = g++

all: $(NAME)

$(NAME) : $(OBJ1) $(OBJ2)
	@ $(GCC) $(FLAGS) $(OBJ1) -o $(NAME1)
	@ $(GCC) $(FLAGS) $(OBJ2) -o $(NAME2)

$(OBJ1) : $(SRC1)
	@ $(GCC) $(FLAGS) -c $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2)
	@ $(GCC) $(FLAGS) -c $(SRC2) -o $(OBJ2)

clean:
	@ rm -rf $(OBJ1) $(OBJ2) $(NAME1) $(NAME2)

fclean:
	@ rm -f $(OBJ1) $(OBJ2) 
