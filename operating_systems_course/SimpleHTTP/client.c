#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 4096

int main() {

  struct addrinfo hints, *res, *p;
  struct sockaddr_in server_address;
  char ipstr[INET_ADDRSTRLEN];
  int client_socket;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo("httpforever.com", "80", &hints, &res) != 0) {
    perror("getaddrinfo error");
    return 1;
  }

  for (p = res; p != NULL; p = p->ai_next) {
    if (p->ai_family == AF_INET) {
      struct sockaddr_in *ip = (struct sockaddr_in *)p->ai_addr;
      inet_ntop(p->ai_family, &(ip->sin_addr), ipstr, sizeof(ipstr));
      server_address = *ip;
      break;
    }
  }
  freeaddrinfo(res);

  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  server_address.sin_port = htons(80);

  if (connect(client_socket, (const struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }
  const char *request = {
      "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n"};
  int sent = send(client_socket, request, strlen(request), 0);
  if (sent == -1) {
    perror("send error");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  int fd = open("httpforever.html", O_CREAT | O_WRONLY | O_TRUNC, 0666);
  if (fd < 0) {
    perror("send error");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  char buf[BUFSIZE];
  int bytes_received;
  while ((bytes_received = recv(client_socket, buf, BUFSIZE, 0)) > 0)
    write(fd, buf, bytes_received);
  if (bytes_received < 0)
    perror("recv error");
  printf("Server answer is saved in file httpforever.html\n");
  close(client_socket);
  close(fd);
}
