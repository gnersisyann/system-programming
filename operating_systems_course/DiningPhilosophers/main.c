#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

sem_t chopsticks[5];
pid_t philosophers[5];
sem_t *print_lock;
volatile sig_atomic_t stop = 0;

void handle_interrupt() { stop = 1; }
void handle_terminate() { exit(0); }

void safe_print(const char *msg, int action) {
  sem_wait(print_lock);
  if (action >= 0 && action <= 4)
    printf("%s (%d)\n", msg, action);
  else
    printf("%s", msg);
  sem_post(print_lock);
}

void philosopher(int id) {
  signal(SIGTERM, handle_terminate);
  srand(time(NULL) ^ id);

  int left = id;
  int right = (id + 1) % 5;
  if (left > right) {
    int temp = left;
    left = right;
    right = temp;
  }

  while (!stop) {
    safe_print("Philosopher is thinking", id);
    sleep(rand() % 3 + 1);

    if (sem_wait(&chopsticks[left]) == 0) {
      if (sem_wait(&chopsticks[right]) == 0) {
        safe_print("Philosopher is eating", id);
        sleep(rand() % 3 + 1);
        sem_post(&chopsticks[right]);
      }
      sem_post(&chopsticks[left]);
    }
  }
  exit(0);
}

void cleanup() {
  printf("Cleaning up...\n");
  for (int i = 0; i < 5; ++i) {
    sem_destroy(&chopsticks[i]);
  }
  sem_destroy(print_lock);
  munmap(print_lock, sizeof(sem_t));
  printf("Done.\n");
}

int main() {
  signal(SIGINT, handle_interrupt);
  signal(SIGTERM, cleanup);
  signal(SIGSEGV, cleanup);

  print_lock = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sem_init(print_lock, 1, 1);

  for (int i = 0; i < 5; ++i) {
    if (sem_init(&chopsticks[i], 1, 1) == -1) {
      perror("sem_init");
      cleanup();
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < 5; ++i) {
    philosophers[i] = fork();
    if (philosophers[i] == -1) {
      perror("Error creating process");
      for (int j = 0; j < i; ++j) {
        kill(philosophers[j], SIGTERM);
        waitpid(philosophers[j], NULL, 0);
      }
      cleanup();
      exit(EXIT_FAILURE);
    }
    if (philosophers[i] == 0) {
      philosopher(i);
    }
  }

  while (!stop) {
    sleep(1);
  }

  for (int i = 0; i < 5; ++i) {
    kill(philosophers[i], SIGTERM);
    waitpid(philosophers[i], NULL, 0);
  }

  cleanup();
  return 0;
}