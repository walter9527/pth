#pragma once

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <stdlib.h>

#include <string.h>

#include <stdarg.h>

#include <errno.h>

#include <dirent.h>

#include <pwd.h>

#include <grp.h>

#include <time.h>

#include <sys/wait.h>

#include <signal.h>

#include <sys/msg.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <sys/sem.h>

#include <pthread.h>

#include <semaphore.h>

extern char **environ;

extern void is_err(ssize_t ret, char *opt); 
extern void is_null_err(void *ret, char *opt); 


// 系统IO函数
extern int my_open(const char *pathname, int flags, ...);
extern int my_close(int fd);
extern ssize_t my_read(int fd, void *buf, size_t count);
extern ssize_t my_write(int fd, const void *buf, size_t count);
extern off_t my_lseek(int fd, off_t offset, int whence);
extern int my_dup(int oldfd);
extern int my_dup2(int oldfd, int newfd);


// 文件属性函数
extern int my_stat(const char *pathname, struct stat *statbuf);
extern int my_lstat(const char *pathname, struct stat *statbuf);
extern int my_fstat(int fd, struct stat *statbuf);
extern int my_truncate(const char *path, off_t length);
extern int my_ftruncate(int filds, off_t length);
extern int my_link(const char *oldpath, const char *newpath);
extern int my_unlink(const char *pathname);
extern int my_remove(const char *pathname);
extern int my_rename(const char *oldpath, const char *newpath);
extern int my_symlink(const char *oldpath, const char *newpath);
extern ssize_t my_readlink(const char *restrict path, char *restrict buf, size_t bufsize);
extern char *my_getcwd(char *buf, size_t size);
//extern char *my_getwd(char *buf);
extern int my_chdir(const char *path);
extern int my_rmdir (const char *path);
extern DIR *my_opendir(const char *name);
extern int my_chmod(const char *path, mode_t mode);

// 标准IO函数
//


// 系统文件
extern struct passwd *my_getpwuid(uid_t uid);
extern struct passwd *my_getpwnam(const char *name);
extern struct group *my_getgrgid(gid_t gid);
extern struct group *my_getgrnam(const char *name);

// 系统时间
extern time_t my_time(time_t *tloc);
extern char *my_ctime(const time_t *timep);
extern struct tm *my_gmtime(const time_t *timep);
extern struct tm *my_localtime(const time_t *timep);
extern time_t my_mktime(struct tm *tm);


// 进程函数
extern pid_t my_fork();
