#ifndef __QOS2_H__
#define __QOS2_H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef UINT32 QOS_FLOWID;
typedef QOS_FLOWID *PQOS_FLOWID;

typedef struct _QOS_VERSION {
    USHORT MajorVersion;
    USHORT MinorVersion;
} QOS_VERSION, *PQOS_VERSION;

typedef enum _QOS_TRAFFIC_TYPE {
    QOSTrafficTypeBestEffort = 0,
    QOSTrafficTypeBackground = 1,
    QOSTrafficTypeExcellentEffort = 2,
    QOSTrafficTypeAudioVideo = 3,
    QOSTrafficTypeVoice = 4,
    QOSTrafficTypeControl = 5
} QOS_TRAFFIC_TYPE;

#ifndef QOS_NON_ADAPTIVE_FLOW
#define QOS_NON_ADAPTIVE_FLOW 0x00000002
#endif

#ifdef __cplusplus
}
#endif

#endif
