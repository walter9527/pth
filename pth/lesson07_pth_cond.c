#include "common.h"

struct globle_va 
{
    pthread_cond_t cond; 
    pthread_mutex_t mutex;
    pthread_t tid;
    int va;
} glbva = {
    .va = 1,
    .cond = PTHREAD_COND_INITIALIZER,
    .mutex = PTHREAD_MUTEX_INITIALIZER
}; 

void print_err(char *str, int line, int err_no)
{
    printf("%d, %s:%s\n", line, str, strerror(err_no));
    exit(-1);
}


void *pth_fun(void *pth_arg)
{
    while (1) {
        pthread_mutex_lock(&glbva.mutex);
        
        // 如果 cond 没有被设置，则该函数会休眠，然后释放锁
        // 如果 cond 条件满足，  则会尝试加锁
        pthread_cond_wait(&glbva.cond, &glbva.mutex);

        printf("va = %d\n", glbva.va);
        glbva.va = 0;

        pthread_mutex_unlock(&glbva.mutex);
    }
    
    return NULL;
}

void signal_fun(int signo)
{
    int ret = pthread_cancel(glbva.tid);
    if (ret != 0) print_err("pthread_cancel fail", __LINE__, ret);
    
    ret = pthread_cond_destroy(&glbva.cond);
    if (ret != 0) print_err("pthread_cond_destroy fail", __LINE__, ret);

    pthread_mutex_destroy(&glbva.mutex);
    exit(-1);
}


int main(int argc, char *argv[])
{
    signal(SIGINT, signal_fun);
    
    int ret = pthread_cond_init(&glbva.cond, NULL); // 初始化条件变量
    if (ret != 0) print_err("pthread_cond_init fail", __LINE__, ret);

    ret = pthread_create(&glbva.tid, NULL, pth_fun, NULL);
    if (ret != 0) print_err("pthread_create fail", __LINE__, ret);

    while (1) {
        pthread_mutex_lock(&glbva.mutex);
        glbva.va++;
        if (glbva.va == 5) {
            pthread_cond_signal(&glbva.cond);
        }
        pthread_mutex_unlock(&glbva.mutex);
        sleep(1);
    }

    return 0;
}
