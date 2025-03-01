NAME = client
CC = cc
CFLAGS = -Wall -Wextra -pthread -lrt
SRC = client.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) httpforever.html

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re