#include "barrera.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int barrier_init(barrier_t *barr, unsigned int count) {
  barr->objetivo = count;
  barr->actual = 0;
  pthread_cond_init(&barr->cond, NULL);
  return count;
}

void barrier_wait(barrier_t *barr) {
  pthread_mutex_lock(&mutex);
  barr->actual++;
  if (barr->actual == barr->objetivo){
    barr->actual = 0;
    pthread_cond_broadcast(&barr->cond);
  } else {
    pthread_cond_wait(&barr->cond, &mutex);
    pthread_mutex_unlock(&mutex);
  }
}

void barrier_destroy(barrier_t *barr) {
  free(barr);
}