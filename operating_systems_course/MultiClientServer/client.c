#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  struct sockaddr_in server_address;

  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("socket creation error");
    exit(errno);
  }

  inet_aton("127.0.0.1", &(server_address.sin_addr));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);

  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("connection failed");
    exit(errno);
  }

  while (1) {
    char *input = NULL;
    size_t size = 0;
    ssize_t len = getline(&input, &size, stdin);
    if (len == -1) {
      perror("getline");
      free(input);
      break;
    }

    int sent = send(client_socket, input, len, 0);
    free(input);

    if (sent == -1) {
      perror("sending error");
      break;
    }
  }

  printf("Disconnected\n");
  close(client_socket);
  return 0;
}