#include "common.h"

pid_t my_fork()
{
    pid_t ret = fork();
    is_err(ret, "fork");
    return ret;
}
