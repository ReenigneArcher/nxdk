#include <assert.h>
#include <stdint.h>

#include <sys/time.h>
#include <sysinfoapi.h>

long timezone = 0;
long _timezone = 0;

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    const uint64_t unixEpochOffset100ns = 116444736000000000ULL;
    FILETIME fileTime;
    uint64_t systemTime100ns;

    assert(tv != NULL);

    if (tz != NULL) {
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }

    GetSystemTimePreciseAsFileTime(&fileTime);
    systemTime100ns = ((uint64_t)fileTime.dwHighDateTime << 32) | fileTime.dwLowDateTime;
    if (systemTime100ns < unixEpochOffset100ns) {
        return -1;
    }

    systemTime100ns -= unixEpochOffset100ns;
    tv->tv_sec = (long)(systemTime100ns / 10000000ULL);
    tv->tv_usec = (long)((systemTime100ns % 10000000ULL) / 10ULL);
    return 0;
}
