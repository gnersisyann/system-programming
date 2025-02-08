#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

sem_t *chopsticks[5];
pid_t philosophers[5];
sem_t *cleanup_lock;
volatile sig_atomic_t stop = 0;

void handle_interrupt() { stop = 1; }

void philosopher(int id) {
  srand(time(NULL));

  int left = id;
  int right = (id + 1) % 5;
  if (id % 2 == 0) {
    int temp = left;
    left = right;
    right = temp;
  }

  while (!stop) {
    printf("Philosopher %d is thinking\n", id);
    sleep(rand() % 3 + 1);

    if (sem_wait(chopsticks[left]) == 0) {
      if (sem_wait(chopsticks[right]) == 0) {
        printf("Philosopher %d is eating\n", id);
        sleep(rand() % 3 + 1);
        sem_post(chopsticks[right]);
      }
      sem_post(chopsticks[left]);
    }
  }
  exit(0);
}

int main() {
  signal(SIGINT, handle_interrupt);
  char sem_name[10];
  cleanup_lock = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (cleanup_lock == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  sem_init(cleanup_lock, 1, 1);

  for (int i = 0; i < 5; ++i) {
    snprintf(sem_name, sizeof(sem_name), "/sem%d", i);
    chopsticks[i] = sem_open(sem_name, O_CREAT | O_EXCL, 0644, 1);
    if (chopsticks[i] == SEM_FAILED) {
      perror("Error opening semaphore");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < 5; ++i) {
    philosophers[i] = fork();
    if (philosophers[i] == -1) {
      perror("Error creating process");
      exit(EXIT_FAILURE);
    }
    if (philosophers[i] == 0) {
      philosopher(i);
    }
  }

  while (!stop) {
    sleep(1);
  }

  printf("\nTerminating philosophers...\n");
  for (int i = 0; i < 5; ++i) {
    kill(philosophers[i], SIGTERM);
    waitpid(philosophers[i], NULL, 0);
  }

  printf("Cleaning up semaphores...\n");
  for (int i = 0; i < 5; ++i) {
    snprintf(sem_name, sizeof(sem_name), "/sem%d", i);
    sem_close(chopsticks[i]);
    sem_unlink(sem_name);
  }

  sem_destroy(cleanup_lock);
  munmap(cleanup_lock, sizeof(sem_t));

  printf("Done.\n");
  return 0;
}
