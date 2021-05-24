#include "common.h"

int my_open(const char *pathname, int flags, ...) {
    int mode = 0;
    if (__OPEN_NEEDS_MODE (flags)) {
        va_list arg;
        va_start (arg, flags);
        mode = va_arg (arg, int);
        va_end (arg);
    }

    int fd = open(pathname, flags, mode);

    is_err(fd, "open");

    return fd;
}

int my_close(int fd) {
    int ret = close(fd);

    is_err(ret, "close");

    return ret;
}

ssize_t my_read(int fd, void *buf, size_t count) {
    ssize_t ret = read(fd, buf, count);

    is_err(ret, "read");

    return ret;
}

ssize_t my_write(int fd, const void *buf, size_t count) {
    ssize_t ret = write(fd, buf, count);

    is_err(ret, "write");

    return ret;
}

off_t my_lseek(int fd, off_t offset, int whence) {
    off_t ret = lseek(fd, offset, whence);

    is_err(ret, "lseek");

    return ret;
}

int my_dup(int oldfd) {
    int ret = dup(oldfd);

    is_err(ret, "dup");

    return ret;
}

int my_dup2(int oldfd, int newfd) {
    int ret = dup2(oldfd, newfd);

    is_err(ret, "dup2");

    return ret;
}
