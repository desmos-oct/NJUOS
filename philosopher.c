#include "thread.h"
#include "thread-sync.h"
#include<unistd.h>

#define N 5
sem_t locks[N];
mutex_t mutex = MUTEX_INIT();
cond_t cv = COND_INIT();

spinlock_t lock = SPIN_INIT();

void Tphilosopher(int id) {
  int lhs = (N + id - 1) % N;
  int rhs = id % N;
  while (1) {
    mutex_lock(&mutex);
    while (!(locks[lhs].__align && locks[rhs].__align)) {
      printf("%d fail\n",id);	    
      cond_wait(&cv, &mutex);
    }
    locks[lhs].__align = locks[rhs].__align = 0;
    printf("%d got %d and %d\n", id, lhs + 1, rhs + 1);
    mutex_unlock(&mutex);

    usleep(10);

    mutex_lock(&mutex);
    locks[lhs].__align = locks[rhs].__align = 1;
    printf("%d release %d and %d\n", id, lhs + 1, rhs + 1);
    cond_broadcast(&cv);
    mutex_unlock(&mutex);
  }
}

int main(int argc, char *argv[]) {
  for (int i = 0; i < N; i++) {
      SEM_INIT(&locks[i], 1);
  }
  for (int i = 0; i < N; i++) {
    create(Tphilosopher);
  }
}