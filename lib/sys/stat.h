#ifndef NXDK_SYS_STAT_H
#define NXDK_SYS_STAT_H

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MODE_T_DEFINED
#define _MODE_T_DEFINED
typedef unsigned int mode_t;
#endif

#ifndef S_IFMT
#define S_IFMT 0170000
#endif

#ifndef _S_IFMT
#define _S_IFMT 0170000
#endif

#ifndef S_IFDIR
#define S_IFDIR 0040000
#endif

#ifndef _S_IFDIR
#define _S_IFDIR 0040000
#endif

#ifndef S_IFCHR
#define S_IFCHR 0020000
#endif

#ifndef _S_IFCHR
#define _S_IFCHR 0020000
#endif

#ifndef S_IFBLK
#define S_IFBLK 0060000
#endif

#ifndef _S_IFBLK
#define _S_IFBLK 0060000
#endif

#ifndef S_IFREG
#define S_IFREG 0100000
#endif

#ifndef _S_IFREG
#define _S_IFREG 0100000
#endif

struct stat {
    unsigned long st_dev;
    mode_t st_mode;
    unsigned long st_ino;
    unsigned int st_nlink;
    unsigned int st_uid;
    unsigned int st_gid;
    unsigned long st_rdev;
    long st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};

int fstat(int fd, struct stat *st);
int stat(const char *filename, struct stat *st);

#ifdef __cplusplus
}
#endif

#ifndef _stat
#define _stat stat
#endif

#ifndef _fstat
#define _fstat fstat
#endif

#endif
