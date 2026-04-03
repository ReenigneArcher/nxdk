#ifndef __GUIDDEF_H__
#define __GUIDDEF_H__

#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GUID {
    DWORD Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[8];
} GUID, *LPGUID;

typedef const GUID *LPCGUID;
typedef const GUID *REFGUID;

#ifdef __cplusplus
}
#endif

#endif
