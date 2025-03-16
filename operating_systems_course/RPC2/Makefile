SERVER = server
CLIENT = client
CC = cc
HEADER = header.h
CFLAGS = -Wall -Wextra -Iparallel_scheduler
LDFLAGS = -Lparallel_scheduler -lparallel_scheduler -pthread
PARALLEL_SCHEDULER_PATH = parallel_scheduler
SRC_CLIENT = client.c
SRC_SERVER = server.c
OBJ_CLIENT = $(SRC_CLIENT:.c=.o)
OBJ_SERVER = $(SRC_SERVER:.c=.o)

all: subsystems $(SERVER) $(CLIENT)

subsystems:
	$(MAKE) -C $(PARALLEL_SCHEDULER_PATH)

$(SERVER): $(OBJ_SERVER)
	$(CC) $^ $(LDFLAGS) -o $@

$(CLIENT): $(OBJ_CLIENT)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(PARALLEL_SCHEDULER_PATH) clean
	rm -f $(OBJ_CLIENT) $(OBJ_SERVER)

fclean: clean
	$(MAKE) -C $(PARALLEL_SCHEDULER_PATH) fclean
	rm -f $(CLIENT) $(SERVER)

re: fclean all

.PHONY: all clean fclean re subsystems