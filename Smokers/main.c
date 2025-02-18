#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define USED '-'
#define TOBACCO 0
#define PAPER 1
#define MATCHES 2
#define RANDOM 1

typedef struct s_info {
  char input[100];
  pid_t person[4];
  sem_t semaphores[3];
  sem_t serving;
  // size_t items[3];
  int finished_smokers;
} t_info;

void kick_all(t_info *info) {
  printf("Kicked all smokers\n");
  for (int i = 0; i < 3; ++i) {
    kill(info->person[i], SIGTERM);
    waitpid(info->person[i], NULL, 0);
  }
  exit(EXIT_SUCCESS);
}

void waiter(t_info *info) {
  // for (int i = 0; i < 3; ++i) {
  //   if (info->items[i] == 0)
  //     info->finished_smokers++;
  // }
  size_t i = 0;
  while (1) {

    while (i < strlen(info->input)) {
      sem_wait(&info->serving);
      printf("\nBartender is searching for items for smokers\n");
      sleep(RANDOM);
      if (info->input[i] == 't') {
        printf("Bartender provides tobacco\n");
        sem_post(&info->semaphores[TOBACCO]);
      } else if (info->input[i] == 'p') {
        printf("Bartender provides paper\n");
        sem_post(&info->semaphores[PAPER]);
      } else if (info->input[i] == 'm') {
        printf("Bartender provides matches\n");
        sem_post(&info->semaphores[MATCHES]);
      } else {
        sem_post(&info->serving);
      }
      info->input[i] = USED;
      i++;
    }
    sleep(RANDOM);
    if (i == strlen(info->input)) {
      kick_all(info);
      exit(EXIT_SUCCESS);
    }
  }
}

void smoker(t_info *info, int id) {
  while (1) {
    sem_wait(&info->semaphores[id]);
    // if (info->items[id] > 0) {
    //   --info->items[id];
    if (id == TOBACCO)
      printf("Smoker %d is using tobacco\n", id);
    else if (id == PAPER)
      printf("Smoker %d is using paper\n", id);
    else if (id == MATCHES)
      printf("Smoker %d is using matches\n", id);
    // }
    sleep(RANDOM);
    sem_post(&info->serving);
    // if (info->items[id] == 0) {
    //   printf("Smoker %d finished smoking and left the bar\n", id);
    //   info->finished_smokers++;
    //   exit(EXIT_SUCCESS);
    // }
  }
}

int main() {
  t_info *info = mmap(NULL, sizeof(t_info), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (info == MAP_FAILED) {
    perror("mmap error");
    exit(EXIT_FAILURE);
  }

  if (scanf("%s", info->input) == EOF) {
    perror("input error");
    munmap(info, sizeof(t_info));
    exit(EXIT_FAILURE);
  }

  if (!strchr(info->input, 't') && !strchr(info->input, 'p') &&
      !strchr(info->input, 'm')) {
    printf("Invalid input\n");
    munmap(info, sizeof(t_info));
    exit(EXIT_FAILURE);
  }

  // info->finished_smokers = 0;
  // info->items[TOBACCO] = 0;
  // info->items[PAPER] = 0;
  // info->items[MATCHES] = 0;
  // for (size_t i = 0; i < strlen(info->input); ++i) {
  //   if (info->input[i] == 't')
  //     ++info->items[TOBACCO];
  //   else if (info->input[i] == 'p')
  //     ++info->items[PAPER];
  //   else if (info->input[i] == 'm')
  //     ++info->items[MATCHES];
  // }

  for (int i = 0; i < 3; ++i)
    sem_init(&info->semaphores[i], 1, 0);
  sem_init(&info->serving, 1, 1);

  srand(time(NULL));
  for (int i = 0; i < 4; ++i) {
    info->person[i] = fork();
    if (info->person[i] < 0) {
      perror("fork error");
      for (int j = i - 1; j >= 0; --j)
        kill(info->person[j], SIGTERM);
      munmap(info, sizeof(t_info));
      exit(errno);
    }
    if (info->person[i] == 0) {
      if (i == 3)
        waiter(info);
      else
        smoker(info, i);
      exit(EXIT_SUCCESS);
    }
  }

  for (int i = 0; i < 4; ++i)
    wait(NULL);

  for (int i = 0; i < 3; ++i)
    sem_destroy(&info->semaphores[i]);
  sem_destroy(&info->serving);

  munmap(info, sizeof(t_info));
}
