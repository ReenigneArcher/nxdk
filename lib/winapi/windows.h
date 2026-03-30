// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2020 Stefan Schmidt
// SPDX-FileCopyrightText: 2020 Jannik Vogel

#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include <debugapi.h>
#include <errhandlingapi.h>
#include <excpt.h>
#include <fibersapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <libloaderapi.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <profileapi.h>
#include <synchapi.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>
#include <winnls.h>
#include <winbase.h>
#include <winerror.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef __ERRNO_T_DEFINED
#define __ERRNO_T_DEFINED
typedef int errno_t;
#endif

static inline errno_t strncpy_s(char *dest, size_t destsz, const char *src, size_t count)
{
	size_t copyLength;
	size_t sourceLength;

	if (dest == NULL || src == NULL || destsz == 0) {
		if (dest != NULL && destsz != 0) {
			dest[0] = '\0';
		}
		return EINVAL;
	}

	sourceLength = strlen(src);
	copyLength = sourceLength;

	if (count != _TRUNCATE && count < copyLength) {
		copyLength = count;
	}

	if (copyLength >= destsz) {
		dest[0] = '\0';
		return ERANGE;
	}

	if (copyLength != 0) {
		memcpy(dest, src, copyLength);
	}
	dest[copyLength] = '\0';

	if (count == _TRUNCATE && sourceLength > copyLength) {
		return ERANGE;
	}

	if (count != _TRUNCATE && sourceLength > copyLength) {
		return ERANGE;
	}

	return 0;
}

static inline errno_t strerror_s(char *buffer, size_t numberOfElements, int errnum)
{
	const char *message;

	if (buffer == NULL || numberOfElements == 0) {
		return EINVAL;
	}

	message = strerror(errnum);
	if (message == NULL) {
		buffer[0] = '\0';
		return EINVAL;
	}

	return strncpy_s(buffer, numberOfElements, message, _TRUNCATE);
}

static inline errno_t mbstowcs_s(size_t *retval, wchar_t *dest, size_t destsz, const char *src, size_t count)
{
	mbstate_t state;
	size_t converted;
	size_t required;
	size_t maxChars;
	const char *source;

	if (retval != NULL) {
		*retval = 0;
	}

	if (dest == NULL || src == NULL || destsz == 0) {
		if (dest != NULL && destsz != 0) {
			dest[0] = L'\0';
		}
		return EINVAL;
	}

	memset(&state, 0, sizeof(state));
	source = src;
	required = mbsrtowcs(NULL, &source, 0, &state);
	if (required == (size_t)-1) {
		dest[0] = L'\0';
		return EILSEQ;
	}

	maxChars = destsz - 1;
	if (count != _TRUNCATE && count < maxChars) {
		maxChars = count;
	}

	memset(&state, 0, sizeof(state));
	source = src;
	converted = mbsrtowcs(dest, &source, maxChars + 1, &state);
	if (converted == (size_t)-1) {
		dest[0] = L'\0';
		return EILSEQ;
	}

	if (converted > maxChars) {
		converted = maxChars;
	}
	dest[converted] = L'\0';

	if (retval != NULL) {
		*retval = converted + 1;
	}

	if (required > maxChars) {
		return ERANGE;
	}

	return 0;
}

static inline void *_malloca(size_t size)
{
	return malloc(size);
}

static inline void _freea(void *memory)
{
	free(memory);
}

static inline DWORD GetEnvironmentVariableW(LPCWSTR name, LPWSTR buffer, DWORD size)
{
	DWORD requiredLength;
	int nameLength;
	char *narrowName;
	const char *value;

	if (name == NULL) {
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	nameLength = WideCharToMultiByte(CP_UTF8, 0, name, -1, NULL, 0, NULL, NULL);
	if (nameLength <= 0) {
		return 0;
	}

	narrowName = (char *)malloc((size_t)nameLength);
	if (narrowName == NULL) {
		SetLastError(ERROR_OUTOFMEMORY);
		return 0;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, name, -1, narrowName, nameLength, NULL, NULL) == 0) {
		free(narrowName);
		return 0;
	}

	value = getenv(narrowName);
	free(narrowName);
	if (value == NULL) {
		SetLastError(ERROR_ENVVAR_NOT_FOUND);
		return 0;
	}

	requiredLength = (DWORD)MultiByteToWideChar(CP_UTF8, 0, value, -1, NULL, 0);
	if (requiredLength == 0) {
		return 0;
	}

	if (buffer == NULL || size == 0) {
		SetLastError(ERROR_SUCCESS);
		return requiredLength;
	}

	if (requiredLength > size) {
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return requiredLength;
	}

	if (MultiByteToWideChar(CP_UTF8, 0, value, -1, buffer, (int)size) == 0) {
		return 0;
	}

	SetLastError(ERROR_SUCCESS);
	return requiredLength - 1;
}

static inline FILE *_wfopen(const wchar_t *filename, const wchar_t *mode)
{
	int filenameLength;
	int modeLength;
	const WCHAR *wideFilename;
	const WCHAR *wideMode;
	char *narrowFilename;
	char *narrowMode;
	FILE *stream;

	if (filename == NULL || mode == NULL) {
		errno = EINVAL;
		return NULL;
	}

#ifdef __cplusplus
	wideFilename = reinterpret_cast<const WCHAR *>(filename);
	wideMode = reinterpret_cast<const WCHAR *>(mode);
#else
	wideFilename = (const WCHAR *)filename;
	wideMode = (const WCHAR *)mode;
#endif

	filenameLength = WideCharToMultiByte(CP_UTF8, 0, wideFilename, -1, NULL, 0, NULL, NULL);
	modeLength = WideCharToMultiByte(CP_UTF8, 0, wideMode, -1, NULL, 0, NULL, NULL);
	if (filenameLength <= 0 || modeLength <= 0) {
		return NULL;
	}

	narrowFilename = (char *)malloc((size_t)filenameLength);
	narrowMode = (char *)malloc((size_t)modeLength);
	if (narrowFilename == NULL || narrowMode == NULL) {
		free(narrowFilename);
		free(narrowMode);
		errno = ENOMEM;
		return NULL;
	}

	if (WideCharToMultiByte(CP_UTF8, 0, wideFilename, -1, narrowFilename, filenameLength, NULL, NULL) == 0 ||
			WideCharToMultiByte(CP_UTF8, 0, wideMode, -1, narrowMode, modeLength, NULL, NULL) == 0) {
		free(narrowFilename);
		free(narrowMode);
		return NULL;
	}

	stream = fopen(narrowFilename, narrowMode);

	free(narrowFilename);
	free(narrowMode);
	return stream;
}

typedef int (*_onexit_t)(void);

static inline _onexit_t _onexit(_onexit_t function)
{
	if (function == NULL) {
		return NULL;
	}

	return atexit((void (*)(void))function) == 0 ? function : NULL;
}

#endif
