#ifndef THREADPOOL_H
#define THREADPOOL_H

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
} thread_pool_t;


void* thread_pool_worker(void* arg);

thread_pool_t* thread_pool_create(size_t num_threads, size_t queue_size);

void thread_pool_submit(thread_pool_t* pool, void (*task)(void*), void* arg);

void thread_pool_shutdown(thread_pool_t* pool);



#endif
