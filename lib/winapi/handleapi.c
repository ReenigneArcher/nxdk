// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019 Stefan Schmidt

#include <handleapi.h>
#include <stdio.h>
#include <pdclib/_PDCLIB_int.h>
#include <winbase.h>
#include <winerror.h>
#include <xboxkrnl/xboxkrnl.h>

HANDLE GetStdHandle (DWORD nStdHandle)
{
  FILE *stream;

  switch (nStdHandle) {
  case STD_INPUT_HANDLE:
    stream = stdin;
    break;
  case STD_OUTPUT_HANDLE:
    stream = stdout;
    break;
  case STD_ERROR_HANDLE:
    stream = stderr;
    break;
  default:
    SetLastError(ERROR_INVALID_PARAMETER);
    return INVALID_HANDLE_VALUE;
  }

  SetLastError(ERROR_SUCCESS);
  return (HANDLE)(ULONG_PTR)stream->handle;
}

DWORD GetFileType (HANDLE hFile)
{
  if (hFile == INVALID_HANDLE_VALUE || hFile == NULL) {
    SetLastError(ERROR_INVALID_HANDLE);
    return FILE_TYPE_UNKNOWN;
  }

  SetLastError(ERROR_SUCCESS);
  return FILE_TYPE_CHAR;
}

BOOL CloseHandle (HANDLE hObject)
{
    NTSTATUS status = NtClose(hObject);

    if (NT_SUCCESS(status)) {
        return TRUE;
    } else {
        SetLastError(RtlNtStatusToDosError(status));
        return FALSE;
    }
}
