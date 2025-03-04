#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096
#define USERNAME_LENGTH 128

void *receive_messages(void *socket_ptr) {
  int client_socket = *(int *)socket_ptr;
  char buffer[BUFSIZE];

  while (1) {
    ssize_t received = read(client_socket, buffer, BUFSIZE - 1);
    if (received <= 0) {
      printf("\nDisconnected from server\n");
      close(client_socket);
      exit(0);
    }
    buffer[received] = '\0';
    printf("%s", buffer);
  }
  return NULL;
}

int main() {
  struct sockaddr_in server_address;
  char username[USERNAME_LENGTH];
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("socket creation error");
    exit(errno);
  }

  server_address.sin_family = AF_INET;
  inet_aton("127.0.0.1", &server_address.sin_addr);
  server_address.sin_port = htons(8888);

  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("connection failed");
    close(client_socket);
    exit(errno);
  }

  printf("Input your username: ");
  fflush(stdout);

  int i = 0;
  while (i < USERNAME_LENGTH - 1) {
    char c = getchar();
    if (c == '\n' || c == EOF)
      break;
    username[i++] = c;
  }
  username[i] = '\0';

  if (write(client_socket, username, i) == -1) {
    perror("send username failed");
    close(client_socket);
    exit(errno);
  }

  pthread_t recv_thread;
  if (pthread_create(&recv_thread, NULL, receive_messages, &client_socket) !=
      0) {
    perror("pthread_create");
    close(client_socket);
    exit(errno);
  }
  pthread_detach(recv_thread);

  char input[BUFSIZE];
  while (1) {
    int pos = 0;
    while (pos < BUFSIZE - 1) {
      char c = getchar();
      if (c == '\n' || c == EOF)
        break;
      input[pos++] = c;
    }
    input[pos] = '\0';

    if (strcmp(input, "/exit") == 0) {
      write(client_socket, "/exit", 5);
      break;
    } else if (strcmp(input, "/list") == 0) {
      write(client_socket, "/list", 5);
    } else if (pos > 0) {
      write(client_socket, input, pos);
    }
  }

  close(client_socket);
  printf("Disconnected\n");
  return 0;
}