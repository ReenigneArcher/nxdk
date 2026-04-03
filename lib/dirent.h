#ifndef NXDK_DIRENT_H
#define NXDK_DIRENT_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <fileapi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dirent {
    char d_name[MAX_PATH];
} dirent;

typedef struct DIR {
    HANDLE handle;
    WIN32_FIND_DATAA findData;
    struct dirent current;
    int firstReadPending;
} DIR;

static inline DIR *opendir(const char *dirname)
{
    DIR *dir;
    size_t length;
    char *searchPattern;

    if (dirname == NULL || dirname[0] == '\0') {
        errno = EINVAL;
        return NULL;
    }

    length = strlen(dirname);
    searchPattern = (char *)malloc(length + 3);
    if (searchPattern == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    memcpy(searchPattern, dirname, length);
    if (dirname[length - 1] == '\\' || dirname[length - 1] == '/') {
        searchPattern[length++] = '*';
    } else {
        searchPattern[length++] = '\\';
        searchPattern[length++] = '*';
    }
    searchPattern[length] = '\0';

    dir = (DIR *)malloc(sizeof(*dir));
    if (dir == NULL) {
        free(searchPattern);
        errno = ENOMEM;
        return NULL;
    }

    dir->handle = FindFirstFileA(searchPattern, &dir->findData);
    free(searchPattern);
    if (dir->handle == INVALID_HANDLE_VALUE) {
        free(dir);
        errno = ENOENT;
        return NULL;
    }

    dir->firstReadPending = 1;
    return dir;
}

static inline struct dirent *readdir(DIR *dir)
{
    if (dir == NULL) {
        errno = EINVAL;
        return NULL;
    }

    if (dir->firstReadPending) {
        dir->firstReadPending = 0;
    } else if (!FindNextFileA(dir->handle, &dir->findData)) {
        return NULL;
    }

    strncpy(dir->current.d_name, dir->findData.cFileName, sizeof(dir->current.d_name) - 1);
    dir->current.d_name[sizeof(dir->current.d_name) - 1] = '\0';
    return &dir->current;
}

static inline int closedir(DIR *dir)
{
    int result;

    if (dir == NULL) {
        errno = EINVAL;
        return -1;
    }

    result = FindClose(dir->handle) ? 0 : -1;
    free(dir);
    return result;
}

#ifdef __cplusplus
}
#endif

#endif
