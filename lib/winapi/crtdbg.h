#ifndef __CRTDBG_H__
#define __CRTDBG_H__

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __analysis_assume
#define __analysis_assume(expr) ((void)0)
#endif

#ifndef _ASSERTE
#define _ASSERTE(expr) assert(expr)
#endif

#ifndef _ASSERT
#define _ASSERT(expr) assert(expr)
#endif

#ifdef __cplusplus
}
#endif

#endif
