#include <stdio.h>
#include "threadpool.h"

// function to be executed by the threads in the pool
void print_message(void *arg) {
    char *message = (char *)arg;
    printf("%s\n", message);
}

int main() {
    // create a threadpool with 4 worker threads
    threadpool_t *pool = threadpool_create(4);
    
    // add tasks to the threadpool
    threadpool_add_task(pool, print_message, "Hello from task 1");
    threadpool_add_task(pool, print_message, "Hello from task 2");
    threadpool_add_task(pool, print_message, "Hello from task 3");
    threadpool_add_task(pool, print_message, "Hello from task 4");
    threadpool_add_task(pool, print_message, "Hello from task 5");
    
    // wait for tasks to complete
    threadpool_wait(pool);
    
    // destroy the threadpool
    threadpool_destroy(pool);
    
    return 0;
}

