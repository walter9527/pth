#include "common.h"

struct passwd *my_getpwuid(uid_t uid)
{
    struct passwd *ret = getpwuid(uid);
    is_null_err(ret, "getpwuid");
    return ret;
}

struct passwd *my_getpwnam(const char *name)
{
    struct passwd *ret = getpwnam(name);
    is_null_err(ret, "getpwnam");
    return ret;
}

struct group *my_getgrgid(gid_t gid)
{
    struct group *ret = getgrgid(gid);
    is_null_err(ret, "getgruid");
    return ret;
}

struct group *my_getgrnam(const char *name)
{
    struct group *ret = getgrnam(name);
    is_null_err(ret, "getgrnam");
    return ret;
}

