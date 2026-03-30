// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2019-2020 Jannik Vogel

#ifndef __WINDEF_H__
#define __WINDEF_H__

#ifndef _WINDEF_H
#define _WINDEF_H
#endif

#include <basetsd.h>
#include <xboxkrnl/xboxdef.h>

#define far
#define FAR far

#define WINAPI   __stdcall
#define CALLBACK WINAPI

#ifndef CONST
#define CONST const
#endif

#define MAX_PATH 260

typedef HANDLE HWND;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

typedef int(FAR WINAPI *FARPROC)(void);

#endif
