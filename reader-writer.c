#include "thread.h"
#include "thread-sync.h"

#define N 5
int reader_count = 0;
mutex_t wlk = MUTEX_INIT();
mutex_t rlk = MUTEX_INIT();
cond_t cv = COND_INIT();

void reader(){
    while(1){
        mutex_lock(&rlk);
        if (reader_count == 0){
            // first reader
            cond_wait(&cv, &rlk);
        }
        mutex_lock(&wlk);
        reader_count++;
        printf("start to read\n");
        reader_count--;
        mutex_unlock(&wlk);
        mutex_unlock(&rlk);
    }
}

void writer(){
    while(1){
        mutex_lock(&wlk);
        printf("start to write\n");
        cond_signal(&cv);
        mutex_unlock(&wlk);
    }
    
}

int main(){
    for (int i = 0; i < N;i++){
        create(reader);
        create(writer);
    }
}