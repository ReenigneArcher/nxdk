// SPDX-License-Identifier: MIT

#ifndef __WINNLS_H__
#define __WINNLS_H__

#include <stddef.h>
#include <string.h>
#include <wchar.h>

#include <winbase.h>
#include <winerror.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CP_ACP
#define CP_ACP 0
#endif

#ifndef CP_UTF8
#define CP_UTF8 65001
#endif

#ifndef MB_ERR_INVALID_CHARS
#define MB_ERR_INVALID_CHARS 0x00000008
#endif

static inline UINT GetACP(void)
{
    return CP_ACP;
}

static inline int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr,
                                      int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
    size_t sourceLength;
    size_t i;

    (void)CodePage;
    (void)dwFlags;

    if (lpMultiByteStr == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    if (cbMultiByte == -1) {
        sourceLength = strlen(lpMultiByteStr) + 1;
    } else if (cbMultiByte >= 0) {
        sourceLength = (size_t)cbMultiByte;
    } else {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    if (lpWideCharStr == NULL) {
        SetLastError(ERROR_SUCCESS);
        return (int)sourceLength;
    }

    if (cchWideChar < (int)sourceLength) {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return 0;
    }

    for (i = 0; i < sourceLength; i++) {
        lpWideCharStr[i] = (WCHAR)(unsigned char)lpMultiByteStr[i];
    }

    SetLastError(ERROR_SUCCESS);
    return (int)sourceLength;
}

static inline int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr,
                                      int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
                                      LPCSTR lpDefaultChar, BOOL *lpUsedDefaultChar)
{
    size_t sourceLength;
    size_t i;

    (void)CodePage;
    (void)dwFlags;
    (void)lpDefaultChar;

    if (lpUsedDefaultChar != NULL) {
        *lpUsedDefaultChar = FALSE;
    }

    if (lpWideCharStr == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    if (cchWideChar == -1) {
        const WCHAR *cursor = lpWideCharStr;
        while (*cursor != 0) {
            cursor++;
        }
        sourceLength = (size_t)(cursor - lpWideCharStr) + 1;
    } else if (cchWideChar >= 0) {
        sourceLength = (size_t)cchWideChar;
    } else {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    if (lpMultiByteStr == NULL) {
        SetLastError(ERROR_SUCCESS);
        return (int)sourceLength;
    }

    if (cbMultiByte < (int)sourceLength) {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return 0;
    }

    for (i = 0; i < sourceLength; i++) {
        WCHAR current = lpWideCharStr[i];
        if (current > 0xFF && lpUsedDefaultChar != NULL) {
            *lpUsedDefaultChar = TRUE;
        }
        lpMultiByteStr[i] = (char)current;
    }

    SetLastError(ERROR_SUCCESS);
    return (int)sourceLength;
}

#ifdef __cplusplus
}
#endif

#endif
