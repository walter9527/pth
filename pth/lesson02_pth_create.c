#include "common.h"

#define PTH_FILE "./file"

#define SUB_PTH_NUMS 2

typedef struct pthread_arg {
    pthread_t tid;
    int pthno; 
    int fd;
} ptharg;


// strerror 能把 errno 翻译成对应的文字
void print_err(char *str, int line, int err_no)
{
    printf("%d, %s:%s\n", line, str, strerror(err_no));
    exit(-1);
}

void *pth_fun(void *pth_arg)
{
    int fd = ((ptharg *)pth_arg)->fd;

    int pthno = ((ptharg *)pth_arg)->pthno;
    pthread_t tid = ((ptharg *)pth_arg)->tid;

    printf("pthno=%d, tid=%lu\n", pthno, tid);

    while (1) {
        write(fd, "hello ", 6);
        sleep(1);
        write(fd, "world\n", 6);
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    ptharg pth_arg[SUB_PTH_NUMS]; // 结构体数组，每个元素会被当做参数传递给对应过的次线程

    // 打开文件，供线程操作，所有的线程（函数）可以共享打开的文件描述符
    int fd = open(PTH_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open file fail", __LINE__, errno);

    /* 通过循环创建两个次线程 */
    for (int i = 0; i < SUB_PTH_NUMS; i++) {

        pth_arg[i].fd = fd; // 保存文件描述符
        pth_arg[i].pthno = i; // 我自己给的线程编号

        int ret = pthread_create(&pth_arg[i].tid, NULL, pth_fun, &pth_arg[i]);
        if (ret != 0) print_err("pthread_create fail", __LINE__, ret);
    }

    while (1) {
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
    }

    return 0;
}
