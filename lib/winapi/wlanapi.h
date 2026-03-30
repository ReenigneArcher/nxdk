#ifndef __WLANAPI_H__
#define __WLANAPI_H__

#include <guiddef.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _WLAN_INTERFACE_STATE {
    wlan_interface_state_not_ready = 0,
    wlan_interface_state_connected = 1,
    wlan_interface_state_ad_hoc_network_formed = 2,
    wlan_interface_state_disconnecting = 3,
    wlan_interface_state_disconnected = 4,
    wlan_interface_state_associating = 5,
    wlan_interface_state_discovering = 6,
    wlan_interface_state_authenticating = 7
} WLAN_INTERFACE_STATE, *PWLAN_INTERFACE_STATE;

typedef enum _WLAN_INTF_OPCODE {
    wlan_intf_opcode_autoconf_enabled = 1,
    wlan_intf_opcode_background_scan_enabled = 2,
    wlan_intf_opcode_media_streaming_mode = 7
} WLAN_INTF_OPCODE, *PWLAN_INTF_OPCODE;

typedef struct _WLAN_INTERFACE_INFO {
    GUID InterfaceGuid;
    WCHAR strInterfaceDescription[256];
    WLAN_INTERFACE_STATE isState;
} WLAN_INTERFACE_INFO, *PWLAN_INTERFACE_INFO;

typedef struct _WLAN_INTERFACE_INFO_LIST {
    DWORD dwNumberOfItems;
    DWORD dwIndex;
    WLAN_INTERFACE_INFO InterfaceInfo[1];
} WLAN_INTERFACE_INFO_LIST, *PWLAN_INTERFACE_INFO_LIST;

#ifdef __cplusplus
}
#endif

#endif
