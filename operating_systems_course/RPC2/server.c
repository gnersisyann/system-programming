#include "header.h"

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_message(const char *message) {
  pthread_mutex_lock(&log_mutex);
  printf("[LOG] %s\n", message);
  pthread_mutex_unlock(&log_mutex);
}

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

void *client_task(void *arg) {
  t_client *client = (t_client *)arg;

  char log_buf[BUFSIZE + 50];
  snprintf(log_buf, sizeof(log_buf), "Received from %d: %s", client->client_fd,
           client->buffer);
  log_message(log_buf);

  client->info.error = 0;
  parser(client->buffer, &client->info);
  calculate(&client->info);

  char res[BUFSIZE];
  if (client->info.error == 1)
    snprintf(res, sizeof(res), "ERROR Division by zero");
  else if (client->info.error == 2)
    snprintf(res, sizeof(res), "ERROR Invalid command");
  else
    snprintf(res, sizeof(res), "%d", client->info.result);

  snprintf(log_buf, sizeof(log_buf), "Sending to %d: %s", client->client_fd,
           res);
  log_message(log_buf);

  send(client->client_fd, res, strlen(res) + 1, 0);

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

  parallel_scheduler *ps = parallel_scheduler_create(MAX_CLIENTS);
  struct pollfd fds[MAX_CLIENTS + 1];
  fds[0].fd = server_fd;
  fds[0].events = POLLIN;
  int nfds = 1;
  while (1) {
    int ret = poll(fds, nfds, -1);
    if (ret < 0) {
      perror("poll");
      break;
    }
    for (int i = 0; i < nfds; i++) {
      if (fds[i].revents & POLLIN) {
        if (fds[i].fd == server_fd) {

          int client_fd = accept(server_fd, NULL, NULL);
          if (client_fd < 0)
            continue;

          if (nfds < MAX_CLIENTS + 1) {
            fds[nfds].fd = client_fd;
            fds[nfds].events = POLLIN;
            nfds++;
          } else {
            const char *msg = "Server is busy try again";
            send(client_fd, msg, strlen(msg) + 1, 0);
            close(client_fd);
            log_message("Refused connection (server busy)");
          }

        } else {

          t_client *client = malloc(sizeof(t_client));
          ssize_t len = recv(fds[i].fd, client->buffer, BUFSIZE - 1, 0);

          if (len <= 0) {
            close(fds[i].fd);
            fds[i] = fds[nfds - 1];
            nfds--;
            i--;
            free(client);
          } else {
            client->buffer[len] = '\0';
            client->client_fd = fds[i].fd;
            parallel_scheduler_run(ps, client_task(client), client);
          }
        }
      }
    }
  }

  parallel_scheduler_destroy(ps);
  close(server_fd);
}