// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#include <winnt.h>
#include <xboxkrnl/xboxkrnl.h>

LONG64 InterlockedExchange64 (LONG64 volatile *Target, LONG64 Value)
{
    // There's no xchg8b in x86, so we implement swapping atomically with cmpxchg8b
    LARGE_INTEGER old_target_value, new_value;
    old_target_value.QuadPart = *Target;
    new_value.QuadPart = Value;

    __asm__ __volatile__("1:\n"
                         "cmpxchg8b (%2)\n"
                         "jnz 1b\n" // If the comparison failed, try again with updated edx:eax
                         : "+a"(old_target_value.LowPart), "+d"(old_target_value.HighPart)
                         : "r"(Target), "b"(new_value.LowPart), "c"(new_value.HighPart)
                         : "memory");

    return old_target_value.QuadPart;
}

LONG64 InterlockedOr64 (LONG64 volatile *Target, LONG64 Value)
{
    LARGE_INTEGER old_target_value, new_value;
    unsigned char exchanged;

    old_target_value.QuadPart = *Target;

    for (;;) {
        new_value.QuadPart = old_target_value.QuadPart | Value;

        __asm__ __volatile__("lock cmpxchg8b (%3)\n"
                             "setz %0\n"
                             : "=q"(exchanged), "+a"(old_target_value.LowPart), "+d"(old_target_value.HighPart)
                             : "r"(Target), "b"(new_value.LowPart), "c"(new_value.HighPart)
                             : "memory");

        if (exchanged) {
            return old_target_value.QuadPart;
        }
    }
}

LONG InterlockedOr (LONG volatile *Target, LONG Value)
{
    LONG old_value;
    LONG new_value;

    do {
        old_value = *Target;
        new_value = old_value | Value;
    } while (InterlockedCompareExchange((PLONG)Target, new_value, old_value) != old_value);

    return old_value;
}

PVOID InterlockedExchangePointer (PVOID volatile *Target, PVOID Value)
{
    return (PVOID)InterlockedExchange((PLONG)Target, (LONG)Value);
}

PVOID InterlockedCompareExchangePointer (PVOID volatile *Destination, PVOID Exchange, PVOID Comperand)
{
    return (PVOID)InterlockedCompareExchange((PLONG)Destination, (LONG)Exchange, (LONG)Comperand);
}
