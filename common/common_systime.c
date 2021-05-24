#include "common.h"

time_t my_time(time_t *tloc)
{
    time_t ret = time(tloc);
    is_err(ret, "time");
    return ret;
}

char *my_ctime(const time_t *timep)
{
    char *ret = ctime(timep);
    is_null_err(ret, "ctime");
    return ret;
}

struct tm *my_gmtime(const time_t *timep)
{
    struct tm *ret = gmtime(timep);
    is_null_err(ret, "gmtime");
    return ret;
}

struct tm *my_localtime(const time_t *timep)
{
    struct tm *ret = localtime(timep);
    is_null_err(ret, "localtime");
    return ret;
}

time_t my_mktime(struct tm *tm)
{
    time_t ret = mktime(tm);
    is_err(ret, "mktime");
    return ret;
}
