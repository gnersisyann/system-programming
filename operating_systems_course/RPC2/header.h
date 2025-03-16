#ifndef HEADER_H
#define HEADER_H

#include "parallel_scheduler/parallel_scheduler.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096

// server
#define MAX_CLIENTS 10

typedef struct s_info {
  int operand; // 0: ADD, 1: SUB, 2: DIV, 3: MUL
  int x;
  int y;
  int result;
  int error; // 0: No error, 1: divide 0, 2: parse error
} t_info;

typedef struct s_client {
  int client_fd;
  char buffer[BUFSIZE];
  t_info info;
} t_client;

#endif