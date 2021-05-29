#include "common.h"

#define PTH_FILE "./file"

#define SUB_PTH_NUMS 2
#define PTH_EXIT -1

typedef struct pthread_arg {
    pthread_t tid;
    int pthno; 
    int fd;
} ptharg;

// 全局变量最好方法到结构体中，避免出现重名的情况
struct globle_va 
{
    ptharg pth_arg[SUB_PTH_NUMS]; // 结构体数组，每个元素会被当做参数传递给对应过的次线程
    int pth_exit_flg[SUB_PTH_NUMS]; // 每个元素存放对应编号线程的退出状态
} glbva;

// strerror 能把 errno 翻译成对应的文字
void print_err(char *str, int line, int err_no)
{
    printf("%d, %s:%s\n", line, str, strerror(err_no));
    exit(-1);
}

/* 线程退出处理函数 */
void pth_exit_deal(void *arg)
{
    pthread_t tid = ((ptharg *)arg)->tid;

    printf("!!!pthread %lu exit\n", tid);
}

void *pth_fun(void *pth_arg)
{
    pthread_detach(pthread_self()); // 分离本线程，结束后自动回收

    int fd = ((ptharg *)pth_arg)->fd;

    int pthno = ((ptharg *)pth_arg)->pthno;
    pthread_t tid = ((ptharg *)pth_arg)->tid;

    // 注册线程退出处理函数
    pthread_cleanup_push(pth_exit_deal, pth_arg);

    printf("pthno=%d, tid=%lu\n", pthno, tid);

    while (1) {
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        // 检测退出状态
        if (glbva.pth_exit_flg[pthno] == PTH_EXIT) break; 
    }
    
    pthread_exit((void *)10); // 可以替代 return

    /***********************************************************************************
    |                                                                                  |
    | 1. 如果线程是执行 pthread_cancel 退出的，即使 pop 没有被执行，仍然会执行退出函数 |
    |                                                                                  |
    | 2. 如果线程是执行 pthread_exit 退出的，即使 pop 没有被执行，仍然会执行退出函数   |
    |    pthread_exit((void *)10);                                                     |
    |    pthread_cleanup_pop(!0); // 会执行退出函数                                    |
    |                                                                                  |
    | 3. 如果线程是执行 return NULL 退出的，pop 没有被执行，不会执行退出函数           |
    |    return NULL;                                                                  |
    |    pthread_cleanup_pop(!0);  // 不会执行退出函数                                 |
    |                                                                                  |
    | 3. 如果线程是执行 return NULL 退出的，pop 有被执行，会执行退出函数               |
    |    pthread_cleanup_pop(!0);  // 会执行退出函数                                   |
    |    return NULL;                                                                  |         
    |                                                                                  |
    | 4. pop 的参数是 0; 不会弹栈，线程不会调用退出处理函数                            |
    |    pthread_cleanup_pop(0);  // 不会执行退出函数                                  |
    ************************************************************************************/
    pthread_cleanup_pop(!0); 
}

void sig_fun(int signo)
{
    if (signo == SIGALRM) {
        for (int i = 0; i < SUB_PTH_NUMS; i++) {
            pthread_cancel(glbva.pth_arg[i].tid);
           // glbva.pth_exit_flg[i] = PTH_EXIT; // 设置为退出状态
        }
    } else if (signo == SIGINT) {
        remove(PTH_FILE);
        exit(0);
    }
}

void process_exit_deal(void)
{
    printf("process exit\n");
}

int main(int argc, char *argv[])
{
    // 注册进程退出处理函数，exit 正常终止进程时弹栈调用
    atexit(process_exit_deal);

    // 打开文件，供线程操作，所有的线程（函数）可以共享打开的文件描述符
    int fd = open(PTH_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open file fail", __LINE__, errno);

    /* 通过循环创建两个次线程 */
    for (int i = 0; i < SUB_PTH_NUMS; i++) {

        glbva.pth_arg[i].fd = fd; // 保存文件描述符
        glbva.pth_arg[i].pthno = i; // 我自己给的线程编号

        int ret = pthread_create(&glbva.pth_arg[i].tid, NULL, pth_fun, &glbva.pth_arg[i]);
        if (ret != 0) print_err("pthread_create fail", __LINE__, ret);
    }

    printf("man tid = %lu\n", pthread_self());

    signal(SIGINT, sig_fun);

    /* 定时 5 秒，时间到后，取消次线程 */
    signal(SIGALRM, sig_fun);
    alarm(3);

#if 0
    void *retval = NULL;
    for (int i = 0; i < SUB_PTH_NUMS; i++) {
        pthread_join(glbva.pth_arg[i].tid, &retval);
        printf("!!!%ld\n", (long)retval);
    }
#endif

    while (1) {
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
    }

    return 0;
}
