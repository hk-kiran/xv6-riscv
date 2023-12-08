#include "kernel/types.h"
#include "user/user.h"
#include "user/thread.h"

#define PGSIZE 4096 // bytes per page

int thread_create(void *(start_routine)(void*), void *arg)
{
    void *stack;
    stack = malloc(PGSIZE);
    int tid = clone(stack);
    if (tid < 0){
        printf("here\n");
        return -1;
    } else if (tid == 0) {
        // child thread
        start_routine(arg);
        // free(stack);
        exit(0);
    }
    return 0;
}

void lock_init(struct lock_t* lock)
{
    lock->locked = 0;
}

void lock_acquire(struct lock_t* lock)
{
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
        ;
    __sync_synchronize();
}

void lock_release(struct lock_t* lock)
{
    __sync_synchronize();
    __sync_lock_release(&lock->locked);
}
