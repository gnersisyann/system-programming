#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFSIZE 4096

typedef struct s_info {
  int operand; // 0: ADD, 1: SUB, 2: DIV, 3: MUL
  int x;
  int y;
  int result;
  int error; // 0: No error, 1: divide 0, 2: parse error
} t_info;

typedef struct s_client {
  int client_socket;
  t_info info;
} t_client;

void parser(char *str, t_info *info) {
  char op[4] = {0};
  info->error = 0;
  if (sscanf(str, "%3s %d %d", op, &info->x, &info->y) != 3) {
    info->error = 2;
    return;
  }

  if (strcmp(op, "ADD") == 0)
    info->operand = 0;
  else if (strcmp(op, "SUB") == 0)
    info->operand = 1;
  else if (strcmp(op, "DIV") == 0)
    info->operand = 2;
  else if (strcmp(op, "MUL") == 0)
    info->operand = 3;
  else
    info->error = 2;
}

void calculate(t_info *info) {
  if (info->error != 0)
    return;

  switch (info->operand) {
  case 0:
    info->result = info->x + info->y;
    break;
  case 1:
    info->result = info->x - info->y;
    break;
  case 2:
    if (info->y == 0)
      info->error = 1;
    else
      info->result = info->x / info->y;
    break;
  case 3:
    info->result = info->x * info->y;
    break;
  default:
    info->error = 2;
    break;
  }
}

void *handle_client(void *args) {
  t_client *client = (t_client *)args;
  char buf[BUFSIZE];

  while (1) {
    ssize_t msg_len = recv(client->client_socket, buf, BUFSIZE - 1, 0);
    if (msg_len <= 0)
      break;
    buf[msg_len] = '\0';

    client->info.error = 0;
    parser(buf, &client->info);
    calculate(&client->info);

    char res[BUFSIZE];
    if (client->info.error == 1)
      snprintf(res, sizeof(res), "ERROR Division by zero");
    else if (client->info.error == 2)
      snprintf(res, sizeof(res), "ERROR Invalid command");
    else
      snprintf(res, sizeof(res), "%d", client->info.result);

    send(client->client_socket, res, strlen(res) + 1, 0);
  }

  close(client->client_socket);
  free(client);
  return NULL;
}

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
    return (perror("socket"), 1);

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_port = htons(8888),
                             .sin_addr.s_addr = INADDR_ANY};

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)))
    return (perror("bind"), 1);

  if (listen(server_fd, MAX_CLIENTS))
    return (perror("listen"), 1);

  printf("Server listening on port 8888...\n");

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
      perror("accept");
      continue;
    }

    t_client *client = malloc(sizeof(t_client));
    if (!client) {
      perror("malloc");
      close(client_fd);
      continue;
    }
    client->client_socket = client_fd;

    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, client);
    pthread_detach(thread);
  }

  close(server_fd);
  return 0;
}