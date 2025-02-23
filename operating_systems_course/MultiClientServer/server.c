#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct s_info {
  int client_socket;
  struct sockaddr_in client_address;
} t_info;

void *handle_connection(void *arg) {
  t_info *info = (t_info *)arg;
  int client_socket = info->client_socket;
  struct sockaddr_in client_address = info->client_address;

  printf("Connected client with address: %s\n",
         inet_ntoa(client_address.sin_addr));

  char buffer[1001];
  int rs;
  while ((rs = recv(client_socket, buffer, 1000, 0)) > 0) {
    buffer[rs] = '\0';
    printf("Got message:\n%s", buffer);
  }
  if (rs == 0)
    printf("Client %s disconnected\n", inet_ntoa(client_address.sin_addr));
  else
    perror("recv error");
  close(client_socket);
  free(info);
  return NULL;
}

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("socket creation error");
    exit(errno);
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    perror("bind failed");
    exit(errno);
  }

  if (listen(server_socket, 10) < 0) {
    perror("listen failed");
    exit(errno);
  }
  printf("Waiting for connection\n");

  while (1) {
    int client_socket;
    struct sockaddr_in client_address;
    unsigned int client_addr_len = sizeof(client_address);

    if ((client_socket =
             accept(server_socket, (struct sockaddr *)&client_address,
                    &client_addr_len)) < 0) {
      perror("accept failed");
      exit(errno);
    }
    pthread_t thread;
    t_info *info = malloc(sizeof(t_info));
    info->client_socket = client_socket;
    info->client_address = client_address;

    if (pthread_create(&thread, NULL, handle_connection, (void *)info) != 0) {
      perror("pthread_create error");
      free(info);
    } else {
      pthread_detach(thread);
    }
  }

  close(server_socket);
  return 0;
}
