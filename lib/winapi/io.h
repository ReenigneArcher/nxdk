#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include <pdclib/_PDCLIB_int.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef _O_TEXT
#define _O_TEXT O_TEXT
#endif

#ifndef _O_BINARY
#define _O_BINARY O_BINARY
#endif

static inline int _close(int fd)
{
	return close(fd);
}

static inline int _read(int fd, void *buf, unsigned int count)
{
	return (int)read(fd, buf, count);
}

static inline int _write(int fd, const void *buf, unsigned int count)
{
	return (int)write(fd, buf, count);
}

static inline long _lseek(int fd, long offset, int whence)
{
	return lseek(fd, offset, whence);
}

static inline int _fileno(FILE *stream)
{
	return (int)stream->handle;
}

static inline int _setmode(int fd, int mode)
{
	(void)fd;
	return mode;
}

static inline int _chmod(const char *path, int mode)
{
	(void)path;
	(void)mode;
	return 0;
}

#endif

