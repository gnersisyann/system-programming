LIBRARY_NAME = libparallel_scheduler.so

NAME = demo-application

FLAGS = -O3 -Wall -std=c++11
LDFLAGS = -L. -lparallel_scheduler -pthread

LIB_SRC = parallel_scheduler.cpp
SRC = main.cpp

LIB_OBJ = parallel_scheduler.o
OBJ = main.o

GCC = g++

$(LIBRARY_NAME): $(LIB_OBJ)
	@ $(GCC) -fPIC $(LIB_OBJ) -shared -o $(LIBRARY_NAME)

$(NAME): $(OBJ) $(LIBRARY_NAME)
	@ $(GCC) $(FLAGS) $(OBJ) $(LDFLAGS) -Wl,-rpath=./ -o $(NAME)

$(LIB_OBJ): $(LIB_SRC)
	@ $(GCC) $(FLAGS) -fPIC -c $(LIB_SRC) -o $(LIB_OBJ)

$(OBJ): $(SRC)
	@ $(GCC) $(FLAGS) -c $(SRC) -o $(OBJ)

clean:
	@ rm -rf $(OBJ) $(LIB_OBJ) $(NAME)

fclean:
	@ rm -f $(OBJ) $(LIB_OBJ) $(NAME) $(LIBRARY_NAME)

re: fclean all
