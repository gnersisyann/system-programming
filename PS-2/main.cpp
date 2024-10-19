#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void split_input(char *input, char **args) {
  char *token = strtok(input, " ");
  int index = 0;
  while (token != nullptr) {
    args[index++] = token;
    token = strtok(nullptr, " ");
  }
  args[index] = nullptr;
}

void do_command(char *input) {
  char *argv[256];
  char *redirect = strchr(input, '>');
  bool append = false;
  if (redirect != nullptr) {
    *redirect = '\0';
    redirect++;
    if (*redirect == '>') {
      append = true;
      *redirect = '\0';
      redirect++;
    }
    while (*redirect == ' ' || *redirect == '\t')
      redirect++;
  }
  pid_t pid = fork();
  if (pid < 0) {
    std::cout << "Fork failed" << std::endl;
    return;
  } else if (pid == 0) {
    if (redirect) {
      int fd;
      if (append)
        fd = open(redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
      else
        fd = open(redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        std::cerr << "Error opening/creating file" << std::endl;
        exit(1);
      }
      dup2(fd, 1);
      close(fd);
    }
    if (strncmp(input, "silent", 6) == 0) {
      int child_pid = getpid();
      char pid_str[20];
      snprintf(pid_str, 20, "%d.log", child_pid);
      int fd = open(pid_str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        std::cerr << "Error opening/creating PID file" << std::endl;
        exit(1);
      }
      dup2(fd, 1);
      dup2(fd, 2);
      close(fd);

      split_input(input + 7, argv);
      execvp(argv[0], argv);
      std::cerr << "Execution failed" << std::endl;
      exit(1);

    } else {
      split_input(input, argv);
      execvp(argv[0], argv);
      std::cerr << "Execution failed" << std::endl;
      exit(1);
    }
  } else {
    wait(NULL);
  }
}

int main() {
  char input[256];
  while (true) {
    std::cout << ">: ";
    std::cin.getline(input, 256);
    if (strcmp(input, "exit") == 0)
      break;
    do_command(input);
  }
}
