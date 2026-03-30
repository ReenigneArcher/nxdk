#ifndef NXDK_UNISTD_H
#define NXDK_UNISTD_H

#include <stddef.h>

#include <synchapi.h>

#define mem_init nxdk_lwip_mem_init
#define mem_trim nxdk_lwip_mem_trim
#define mem_malloc nxdk_lwip_mem_malloc
#define mem_calloc nxdk_lwip_mem_calloc
#define mem_free nxdk_lwip_mem_free
#include "net/lwip/src/include/lwip/sockets.h"
#undef mem_free
#undef mem_calloc
#undef mem_malloc
#undef mem_trim
#undef mem_init
#ifdef accept
#undef accept
#endif
#ifdef bind
#undef bind
#endif
#ifdef shutdown
#undef shutdown
#endif
#ifdef getpeername
#undef getpeername
#endif
#ifdef getsockname
#undef getsockname
#endif
#ifdef setsockopt
#undef setsockopt
#endif
#ifdef getsockopt
#undef getsockopt
#endif
#ifdef closesocket
#undef closesocket
#endif
#ifdef connect
#undef connect
#endif
#ifdef listen
#undef listen
#endif
#ifdef recv
#undef recv
#endif
#ifdef recvmsg
#undef recvmsg
#endif
#ifdef recvfrom
#undef recvfrom
#endif
#ifdef send
#undef send
#endif
#ifdef sendmsg
#undef sendmsg
#endif
#ifdef sendto
#undef sendto
#endif
#ifdef socket
#undef socket
#endif
#ifdef select
#undef select
#endif
#ifdef poll
#undef poll
#endif
#ifdef ioctl
#undef ioctl
#endif
#ifdef ioctlsocket
#undef ioctlsocket
#endif

#ifdef __cplusplus
extern "C" {
#endif

int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
long lseek(int fd, long offset, int whence);

typedef unsigned int useconds_t;

static inline unsigned int sleep(unsigned int seconds)
{
	Sleep(seconds * 1000U);
	return 0;
}

static inline int usleep(useconds_t usec)
{
	Sleep((usec + 999U) / 1000U);
	return 0;
}

static inline int getpid(void)
{
	return 1;
}

static inline int getuid(void)
{
	return 0;
}

static inline int geteuid(void)
{
	return 0;
}

static inline int getgid(void)
{
	return 0;
}

static inline int getegid(void)
{
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif
