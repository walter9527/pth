#include <common.h>

void *pth_fun(void *pth_arg)
{
    while (1) {
        printf("111\n");
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t TID = 0;

    pthread_create(&TID, NULL, pth_fun, NULL);

    while (1) {
        printf("222\n");
        sleep(2);
    }
    return 0;
}
