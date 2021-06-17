#include "common.h"

#define SUB_PTH_NUMS 2 // 次线程数量
#define SEM_NUMS 3 // 集合中信号量数量

#define PTH_FILE "./file"


/* 传递给线程的参数 */
typedef struct pthread_arg {
    pthread_t tid; // 存放线程 tid
    int pthno; // 我自己定义的编号
    int fd; // 文件描述符
} ptharg;


// 全局变量最好方法到结构体中，避免出现重名的情况
struct globle_va 
{
    ptharg pth_arg[SUB_PTH_NUMS]; // 结构体数组，每个元素会被当做参数传递给对应过的次线程
    sem_t sem[SEM_NUMS];
} glbva; 

// strerror 能把 errno 翻译成对应的文字
void print_err(char *str, int line, int err_no)
{
    printf("%d, %s:%s\n", line, str, strerror(err_no));
    exit(-1);
}


void *pth_fun1(void *pth_arg)
{

    while (1) {
        sem_wait(&glbva.sem[0]);
        printf("111111\n");
        sleep(1);
        sem_post(&glbva.sem[1]);
    }
    
    return NULL;
}

void *pth_fun2(void *pth_arg)
{

    while (1) {
        sem_wait(&glbva.sem[1]);
        printf("222222\n");
        sleep(1);
        sem_post(&glbva.sem[2]);
    }
    
    return NULL;
}

void signal_fun(int sigflg)
{
    /* 删除信号量集合 */
    for (int i = 0, ret = 0; i < SEM_NUMS; i++) {
        ret = sem_destroy(&glbva.sem[i]);
        if (ret != 0) print_err("sem destroy fail", __LINE__, errno);
    }

    exit(-1);
}

int main(int argc, char *argv[])
{

    signal(SIGINT, signal_fun);

    void *(*pth_fun_buf[])(void *) = {pth_fun1, pth_fun2};

    /* 初始化信号量 */
    for (int i = 0, ret = 0; i < SEM_NUMS; i++) {
        ret = i == 0 ? sem_init(&glbva.sem[i], 0, 1) : sem_init(&glbva.sem[i], 0, 0);
        if (ret != 0) print_err("sem init fail", __LINE__, errno);
    }

    /* 通过循环创建两个次线程 */
    for (int i = 0; i < SUB_PTH_NUMS; i++) {
        int ret = pthread_create(&glbva.pth_arg[i].tid, NULL, pth_fun_buf[i], NULL);
        if (ret != 0) print_err("pthread_create fail", __LINE__, ret);
    }


    while (1) {
        sem_wait(&glbva.sem[2]);
        printf("333333\n");
        sleep(1);
        sem_post(&glbva.sem[0]);
    }

    return 0;
}
