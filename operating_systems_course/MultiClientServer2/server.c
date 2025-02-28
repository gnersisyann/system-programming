#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define CLIENTS 10
#define BUFSIZE 4096
#define USERNAME_LENGTH 128

typedef struct s_client {
  int client_socket;
  int active;
  char username[USERNAME_LENGTH];
} client;

typedef struct s_info {
  client clients[CLIENTS];
  pthread_mutex_t mutex;
  int server_socket;
} info;

int find_free_client_slot(info *server) {
  for (int i = 0; i < CLIENTS; i++) {
    if (!server->clients[i].active) {
      return i;
    }
  }
  return -1;
}

void send_to_all(const char *buf, int sender_id, info *server) {
  pthread_mutex_lock(&server->mutex);
  for (int i = 0; i < CLIENTS; i++) {
    if (server->clients[i].active && i != sender_id) {
      if (send(server->clients[i].client_socket, buf, strlen(buf), 0) < 0) {
        perror("Send error");
      }
    }
  }
  pthread_mutex_unlock(&server->mutex);
}

void send_user_list(int client_socket, info *server) {
  char list[BUFSIZE] = "Online users:\n";
  pthread_mutex_lock(&server->mutex);
  for (int i = 0; i < CLIENTS; i++) {
    if (server->clients[i].active) {
      strcat(list, server->clients[i].username);
      strcat(list, "\n");
    }
  }
  pthread_mutex_unlock(&server->mutex);
  send(client_socket, list, strlen(list), 0);
}

void exit_handler(info **server, int client_socket, int client_slot) {
  close(client_socket);
  pthread_mutex_lock(&(*server)->mutex);
  (*server)->clients[client_slot].active = 0;
  (*server)->clients[client_slot].client_socket = -1;
  pthread_mutex_unlock(&(*server)->mutex);
  pthread_exit(NULL);
}

typedef struct {
  info *server;
  int client_slot;
  int client_socket;
} thread_arg;

void *client_handler(void *arg) {
  thread_arg *targ = (thread_arg *)arg;
  info *server = targ->server;
  int client_slot = targ->client_slot;
  int client_socket = targ->client_socket;
  free(targ);

  pthread_mutex_lock(&server->mutex);
  server->clients[client_slot].client_socket = client_socket;
  server->clients[client_slot].active = 1;
  pthread_mutex_unlock(&server->mutex);

  ssize_t received_bytes;
  received_bytes = recv(client_socket, server->clients[client_slot].username,
                        USERNAME_LENGTH, 0);
  if (received_bytes < 0) {
    perror("Receive username error");
    exit_handler(&server, client_socket, client_slot);
  }
  server->clients[client_slot].username[received_bytes] = '\0';

  char buf[BUFSIZE];
  while (1) {
    received_bytes = recv(client_socket, buf, BUFSIZE, 0);
    if (received_bytes <= 0) {
      break;
    }
    buf[received_bytes] = '\0';

    if (strcmp(buf, "/exit") == 0) {
      break;
    } else if (strcmp(buf, "/list") == 0) {
      send_user_list(client_socket, server);
    } else {
      char message[BUFSIZE + USERNAME_LENGTH + 2];
      snprintf(message, sizeof(message), "%s: %s\n",
               server->clients[client_slot].username, buf);
      message[sizeof(message) - 1] = '\0';
      send_to_all(message, client_slot, server);
    }
  }

  exit_handler(&server, client_socket, client_slot);
  return NULL;
}

int main() {
  int server_socket;
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Server socket error");
    exit(errno);
  }

  struct sockaddr_in server_info;
  server_info.sin_family = AF_INET;
  server_info.sin_addr.s_addr = htonl(INADDR_ANY);
  server_info.sin_port = htons(8888);

  if (bind(server_socket, (struct sockaddr *)&server_info,
           sizeof(server_info)) < 0) {
    perror("Server bind error");
    exit(errno);
  }

  if (listen(server_socket, CLIENTS) < 0) {
    perror("Server listen error");
    exit(errno);
  }

  info server;
  server.server_socket = server_socket;
  pthread_mutex_init(&server.mutex, NULL);
  for (int i = 0; i < CLIENTS; i++) {
    server.clients[i].active = 0;
    server.clients[i].client_socket = -1;
  }

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
      perror("Accept error");
      continue;
    }

    pthread_mutex_lock(&server.mutex);
    int client_slot = find_free_client_slot(&server);
    pthread_mutex_unlock(&server.mutex);
    if (client_slot < 0) {
      printf("No available client slot\n");
      close(client_socket);
      continue;
    }

    thread_arg *targ = malloc(sizeof(thread_arg));
    if (!targ) {
      perror("Malloc error");
      close(client_socket);
      continue;
    }
    targ->server = &server;
    targ->client_slot = client_slot;
    targ->client_socket = client_socket;

    pthread_t thread;
    if (pthread_create(&thread, NULL, client_handler, (void *)targ) != 0) {
      perror("Pthread create error");
      free(targ);
      close(client_socket);
      continue;
    }
    pthread_detach(thread);
  }

  pthread_mutex_destroy(&server.mutex);
  return 0;
}