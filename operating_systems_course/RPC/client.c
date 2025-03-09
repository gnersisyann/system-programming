#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFSIZE 4096

int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in serv_addr = {.sin_family = AF_INET,
                                  .sin_port = htons(8888)};

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    return (perror("inet_pton"), 1);

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    return (perror("connect"), 1);

  printf("Connected to server. Available command list\nADD <op1> <op2>\n"
         "MUL <op1> <op2>\nSUB <op1> <op2>\nDIV <op1> <op2>\n");

  char buf[BUFSIZE];
  while (1) {
    printf("> ");
    fflush(stdout);

    if (!fgets(buf, BUFSIZE, stdin))
      break;
    buf[strcspn(buf, "\n")] = '\0';

    if (send(sock, buf, strlen(buf) + 1, 0) <= 0) {
      perror("send");
      break;
    }

    char res[BUFSIZE];
    ssize_t len = recv(sock, res, BUFSIZE - 1, 0);
    if (len <= 0) {
      perror("recv");
      break;
    }
    res[len] = '\0';
    printf("Result: %s\n", res);
  }

  close(sock);
  return 0;
}