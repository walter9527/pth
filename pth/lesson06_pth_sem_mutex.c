#include "common.h"

#define SUB_PTH_NUMS 2 // 次线程数量
#define SEM_NUMS 1 // 集合中信号量数量

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


void *pth_fun(void *pth_arg)
{
    // pthread_detach(pthread_self()); // 分离本线程，结束后自动回收，当设置了 detached 属性后，这里可以不要

    int fd = ((ptharg *)pth_arg)->fd;

    while (1) {
        sem_wait(&glbva.sem[0]);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        sem_post(&glbva.sem[0]);
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

    // 打开文件，供线程操作，所有的线程（函数）可以共享打开的文件描述符
    int fd = open(PTH_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open file fail", __LINE__, errno);

    /* 初始化信号量 */
    for (int i = 0, ret = 0; i < SEM_NUMS; i++) {
        ret = i == 0 ? sem_init(&glbva.sem[i], 0, 1) : sem_init(&glbva.sem[i], 0, 0);
        if (ret != 0) print_err("sem init fail", __LINE__, errno);
    }

    /* 通过循环创建两个次线程 */
    for (int i = 0; i < SUB_PTH_NUMS; i++) {

        glbva.pth_arg[i].fd = fd; // 保存文件描述符
        glbva.pth_arg[i].pthno = i; // 我自己给的线程编号

        int ret = pthread_create(&glbva.pth_arg[i].tid, NULL, pth_fun, &glbva.pth_arg[i]);
        if (ret != 0) print_err("pthread_create fail", __LINE__, ret);
    }


    while (1) {
        sem_wait(&glbva.sem[0]);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        sem_post(&glbva.sem[0]);
    }

    return 0;
}
