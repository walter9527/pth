#include "common.h"

#define SEM_NUMS 2 // 集合中信号量数量


// 全局变量最好方法到结构体中，避免出现重名的情况
struct globle_va 
{
    pthread_t tid; // 存放线程 tid
    sem_t sem[SEM_NUMS];
    int va;
} glbva = {.va = 0}; 

// strerror 能把 errno 翻译成对应的文字
void print_err(char *str, int line, int err_no)
{
    printf("%d, %s:%s\n", line, str, strerror(err_no));
    exit(-1);
}


void *pth_fun(void *pth_arg)
{
    while (1) {
        sem_wait(&glbva.sem[1]);
        printf("va = %d\n", glbva.va);
        glbva.va = 0;
        sem_post(&glbva.sem[0]);
    }
    
    return NULL;
}


int main(int argc, char *argv[])
{
    /* 初始化信号量 */
    for (int i = 0, ret = 0; i < SEM_NUMS; i++) {
        ret = i == 0 ? sem_init(&glbva.sem[i], 0, 1) : sem_init(&glbva.sem[i], 0, 0);
        if (ret != 0) print_err("sem init fail", __LINE__, errno);
    }

    int ret = pthread_create(&glbva.tid, NULL, pth_fun, NULL);
    if (ret != 0) print_err("pthread_create fail", __LINE__, ret);


    while (1) {
        sem_wait(&glbva.sem[0]);
        glbva.va++;
        sleep(1);
        if (glbva.va == 5) {
            sem_post(&glbva.sem[1]);
        } else {
            sem_post(&glbva.sem[0]);
        }
    }

    return 0;
}
