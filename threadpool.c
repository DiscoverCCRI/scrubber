#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

typedef struct {
    void (*task)(void*);
    void* arg;
} task_t;

typedef struct {
    task_t* buffer;
    size_t size;
    size_t head;
    size_t tail;
    size_t count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    bool stop;
    pthread_t* threads;
    size_t num_threads;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    size_t queue_size;
    size_t active_threads;
} thread_pool_t;

void* thread_pool_worker(void* arg) {
    thread_pool_t* pool = arg;
    while (true) {
        pthread_mutex_lock(&pool->lock);
        while (pool->count == 0 && !pool->stop) {
            pthread_cond_wait(&pool->not_empty, &pool->lock);
        }
        if (pool->stop) {
            pthread_mutex_unlock(&pool->lock);
            pthread_mutex_lock(&pool->mutex);
            pool->active_threads--;
            if (pool->active_threads == 0) {
                pthread_cond_broadcast(&pool->cond);
            }
            pthread_mutex_unlock(&pool->mutex);
            pthread_exit(NULL);
        }
        assert(pool->count > 0);
        pool->active_threads++;
        task_t task = pool->buffer[pool->head];
        pool->head = (pool->head + 1) % pool->size;
        pool->count--;
        pthread_cond_signal(&pool->not_full);
        pthread_mutex_unlock(&pool->lock);
        task.task(task.arg);
        pthread_mutex_lock(&pool->mutex);
        pool->active_threads--;
        if (pool->active_threads == 0) {
            pthread_cond_broadcast(&pool->cond);
        }
        pthread_mutex_unlock(&pool->mutex);
    }
}

thread_pool_t* thread_pool_create(size_t num_threads, size_t queue_size) {
    thread_pool_t* pool = malloc(sizeof(thread_pool_t));
    pool->buffer = malloc(queue_size * sizeof(task_t));
    pool->size = queue_size;
    pool->head = 0;
    pool->tail = 0;
    pool->count = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->not_empty, NULL);
    pthread_cond_init(&pool->not_full, NULL);
    pool->stop = false;
    pool->threads = malloc(num_threads * sizeof(pthread_t));
    pool->num_threads = num_threads;
    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, thread_pool_worker, pool);
    }
    return pool;
}

void thread_pool_submit(thread_pool_t* pool, void (*task)(void*), void* arg) {
    pthread_mutex_lock(&pool->lock);
    while (pool->count == pool->size && !pool->stop) {
        pthread_cond_wait(&pool->not_full, &pool->lock);
    }
    if (pool->stop) {
        pthread_mutex_unlock(&pool->lock);
        return;
    }
    assert(pool->count < pool->size);
    size_t tail = pool->tail;
    pool->buffer[tail].task = task;
    pool->buffer[tail].arg = arg;
    pool->tail = (tail + 1) % pool->size;
    pool->count++;
    pthread_cond_signal(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);
}
void thread_pool_shutdown(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->stop = true;
    pthread_cond_broadcast(&pool->not_empty);
    pthread_mutex_unlock(&pool->lock);
    for (size_t i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    free(pool->threads);
    free(pool->buffer);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->not_empty);
    pthread_cond_destroy(&pool->not_empty);
    free(pool);


}

void thread_pool_wait(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->mutex);
    while (pool->active_threads > 0) {
        pthread_cond_wait(&pool->cond, &pool->mutex);
    }
    pthread_mutex_unlock(&pool->mutex);
}


// function to be executed by the threads in the pool
void print_message(void *arg) {
    char *message = (char *)arg;
    printf("%s\n", message);
}

void check_prime(void *arg) {
    uint64_t n = *(uint64_t *)arg;
    int is_prime = 1;
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            is_prime = 0;
            break;
        }
    }
    printf("%lu is %s\n", n, is_prime ? "prime" : "not prime");
}

int main() {
    // create a threadpool with 4 worker threads
    thread_pool_t *pool = thread_pool_create(3, 1024);
    
    uint64_t n = 1234567890123456789;
    uint64_t m = 9223372036854775803;

    thread_pool_submit(pool, check_prime, n);

    // add tasks to the threadpool
    //thread_pool_submit(pool, print_message, "Hello from task 1");
    //thread_pool_submit(pool, print_message, "Hello from task 2");
    //thread_pool_submit(pool, print_message, "Hello from task 3");
    //thread_pool_submit(pool, print_message, "Hello from task 4");
    //thread_pool_submit(pool, print_message, "Hello from task 5");
    /*for (int i = 0; i < 1024; i++) {
        thread_pool_submit(pool, print_message, "Hello from task");
    }*/
    
    // wait for tasks to complete
    thread_pool_wait(pool);
    //sleep(10);
    // destroy the threadpool
    thread_pool_shutdown(pool);
    
    return 0;
}



