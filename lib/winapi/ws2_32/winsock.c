#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>

#include <mswsock.h>
#include <winsock2.h>

static thread_local int wsaLastError = 0;

const GUID WSAID_WSARECVMSG = {0xf689d7c8, 0x6f1f, 0x436b, {0x8a, 0x53, 0xe5, 0x4f, 0xe3, 0x51, 0xc3, 0x22}};
const GUID WSAID_WSASENDMSG = {0xa441e712, 0x754f, 0x43ca, {0x84, 0xa7, 0x0d, 0xe5, 0x0d, 0xb8, 0x5e, 0xb2}};

static int nxdk_wsa_error_from_errno(int err)
{
    switch (err) {
    case 0:
        return 0;
#ifdef EINTR
    case EINTR:
        return WSAEINTR;
#endif
#ifdef EBADF
    case EBADF:
        return WSAEBADF;
#endif
#ifdef EACCES
    case EACCES:
        return WSAEACCES;
#endif
#ifdef EFAULT
    case EFAULT:
        return WSAEFAULT;
#endif
#ifdef EINVAL
    case EINVAL:
        return WSAEINVAL;
#endif
#ifdef EMFILE
    case EMFILE:
        return WSAEMFILE;
#endif
#ifdef EWOULDBLOCK
    case EWOULDBLOCK:
        return WSAEWOULDBLOCK;
#endif
#ifdef EAGAIN
    case EAGAIN:
        return WSAEWOULDBLOCK;
#endif
#ifdef EINPROGRESS
    case EINPROGRESS:
        return WSAEINPROGRESS;
#endif
#ifdef EALREADY
    case EALREADY:
        return WSAEALREADY;
#endif
#ifdef ENOTSOCK
    case ENOTSOCK:
        return WSAENOTSOCK;
#endif
#ifdef EDESTADDRREQ
    case EDESTADDRREQ:
        return WSAEDESTADDRREQ;
#endif
#ifdef EMSGSIZE
    case EMSGSIZE:
        return WSAEMSGSIZE;
#endif
#ifdef EPROTOTYPE
    case EPROTOTYPE:
        return WSAEPROTOTYPE;
#endif
#ifdef ENOPROTOOPT
    case ENOPROTOOPT:
        return WSAENOPROTOOPT;
#endif
#ifdef EPROTONOSUPPORT
    case EPROTONOSUPPORT:
        return WSAEPROTONOSUPPORT;
#endif
#ifdef ESOCKTNOSUPPORT
    case ESOCKTNOSUPPORT:
        return WSAESOCKTNOSUPPORT;
#endif
#ifdef EOPNOTSUPP
    case EOPNOTSUPP:
        return WSAEOPNOTSUPP;
#endif
#ifdef EPFNOSUPPORT
    case EPFNOSUPPORT:
        return WSAEPFNOSUPPORT;
#endif
#ifdef EAFNOSUPPORT
    case EAFNOSUPPORT:
        return WSAEAFNOSUPPORT;
#endif
#ifdef EADDRINUSE
    case EADDRINUSE:
        return WSAEADDRINUSE;
#endif
#ifdef EADDRNOTAVAIL
    case EADDRNOTAVAIL:
        return WSAEADDRNOTAVAIL;
#endif
#ifdef ENETDOWN
    case ENETDOWN:
        return WSAENETDOWN;
#endif
#ifdef ENETUNREACH
    case ENETUNREACH:
        return WSAENETUNREACH;
#endif
#ifdef ENETRESET
    case ENETRESET:
        return WSAENETRESET;
#endif
#ifdef ECONNABORTED
    case ECONNABORTED:
        return WSAECONNABORTED;
#endif
#ifdef ECONNRESET
    case ECONNRESET:
        return WSAECONNRESET;
#endif
#ifdef ENOBUFS
    case ENOBUFS:
        return WSAENOBUFS;
#endif
#ifdef EISCONN
    case EISCONN:
        return WSAEISCONN;
#endif
#ifdef ENOTCONN
    case ENOTCONN:
        return WSAENOTCONN;
#endif
#ifdef ESHUTDOWN
    case ESHUTDOWN:
        return WSAESHUTDOWN;
#endif
#ifdef ETIMEDOUT
    case ETIMEDOUT:
        return WSAETIMEDOUT;
#endif
#ifdef ECONNREFUSED
    case ECONNREFUSED:
        return WSAECONNREFUSED;
#endif
#ifdef ELOOP
    case ELOOP:
        return WSAELOOP;
#endif
#ifdef ENAMETOOLONG
    case ENAMETOOLONG:
        return WSAENAMETOOLONG;
#endif
#ifdef EHOSTDOWN
    case EHOSTDOWN:
        return WSAEHOSTDOWN;
#endif
#ifdef EHOSTUNREACH
    case EHOSTUNREACH:
        return WSAEHOSTUNREACH;
#endif
#ifdef ENOTEMPTY
    case ENOTEMPTY:
        return WSAENOTEMPTY;
#endif
#ifdef EUSERS
    case EUSERS:
        return WSAEUSERS;
#endif
#ifdef EDQUOT
    case EDQUOT:
        return WSAEDQUOT;
#endif
#ifdef ESTALE
    case ESTALE:
        return WSAESTALE;
#endif
#ifdef EREMOTE
    case EREMOTE:
        return WSAEREMOTE;
#endif
    default:
        return err >= WSABASEERR ? err : WSAEINVAL;
    }
}

static void nxdk_wsa_set_last_error_from_errno(void)
{
    wsaLastError = nxdk_wsa_error_from_errno(errno);
}

static int nxdk_guid_equal(const GUID *left, const GUID *right)
{
    return memcmp(left, right, sizeof(*left)) == 0;
}

static void nxdk_wsamsg_to_msghdr(const WSAMSG *source, struct msghdr *target)
{
    target->msg_name = source->name;
    target->msg_namelen = (socklen_t)source->namelen;
    target->msg_iov = (struct iovec *)source->lpBuffers;
    target->msg_iovlen = (msg_iovlen_t)source->dwBufferCount;
    target->msg_control = source->Control.buf;
    target->msg_controllen = (socklen_t)source->Control.len;
    target->msg_flags = (int)source->dwFlags;
}

static void nxdk_msghdr_to_wsamsg(const struct msghdr *source, WSAMSG *target)
{
    target->namelen = (INT)source->msg_namelen;
    target->Control.len = (ULONG)source->msg_controllen;
    target->dwFlags = (DWORD)source->msg_flags;
}

static int nxdk_resolve_service_port(const char *name)
{
    static const struct {
        const char *name;
        int port;
    } services[] = {
        {"http", 80},
        {"https", 443},
        {"ldap", 389},
        {"ldaps", 636},
        {"smtp", 25},
        {"submission", 587},
        {"imap", 143},
        {"imaps", 993},
        {"pop3", 110},
        {"pop3s", 995},
    };
    size_t i;

    if (name == NULL) {
        return -1;
    }

    for (i = 0; i < sizeof(services) / sizeof(services[0]); i++) {
        if (strcmp(name, services[i].name) == 0) {
            return services[i].port;
        }
    }

    return -1;
}

int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen,
                char *serv, socklen_t servlen, int flags)
{
    (void)flags;

    if (addr == NULL) {
        return EAI_FAIL;
    }

    if (host != NULL && hostlen > 0) {
        const void *sourceAddress;
        int family = addr->sa_family;

        if (family == AF_INET) {
            if (addrlen < (socklen_t)sizeof(struct sockaddr_in)) {
                return EAI_FAIL;
            }
            sourceAddress = &((const struct sockaddr_in *)addr)->sin_addr;
        }
#ifdef AF_INET6
        else if (family == AF_INET6) {
            if (addrlen < (socklen_t)sizeof(struct sockaddr_in6)) {
                return EAI_FAIL;
            }
            sourceAddress = &((const struct sockaddr_in6 *)addr)->sin6_addr;
        }
#endif
        else {
            return EAI_FAMILY;
        }

        if (inet_ntop(family, sourceAddress, host, hostlen) == NULL) {
            return EAI_FAIL;
        }
    }

    if (serv != NULL && servlen > 0) {
        unsigned short port;
        int written;

        if (addr->sa_family == AF_INET) {
            if (addrlen < (socklen_t)sizeof(struct sockaddr_in)) {
                return EAI_FAIL;
            }
            port = ntohs(((const struct sockaddr_in *)addr)->sin_port);
        }
#ifdef AF_INET6
        else if (addr->sa_family == AF_INET6) {
            if (addrlen < (socklen_t)sizeof(struct sockaddr_in6)) {
                return EAI_FAIL;
            }
            port = ntohs(((const struct sockaddr_in6 *)addr)->sin6_port);
        }
#endif
        else {
            return EAI_FAMILY;
        }

        written = snprintf(serv, servlen, "%u", port);
        if (written < 0 || written >= servlen) {
            return EAI_FAIL;
        }
    }

    return 0;
}

const char *gai_strerror(int errcode)
{
    switch (errcode) {
    case 0:
        return "Success";
    case EAI_NONAME:
        return "Name or service not known";
    case EAI_SERVICE:
        return "Service not supported";
    case EAI_FAIL:
        return "Non-recoverable failure in name resolution";
    case EAI_MEMORY:
        return "Memory allocation failure";
    case EAI_FAMILY:
        return "Address family not supported";
    default:
        return "Unknown resolver error";
    }
}

struct servent *getservbyname(const char *name, const char *proto)
{
    static char *aliases[] = {NULL};
    static struct servent service;
    int port;

    if (proto != NULL && strcmp(proto, "tcp") != 0 && strcmp(proto, "udp") != 0) {
        return NULL;
    }

    port = nxdk_resolve_service_port(name);
    if (port < 0) {
        return NULL;
    }

    service.s_name = (char *)name;
    service.s_aliases = aliases;
    service.s_port = htons((unsigned short)port);
    service.s_proto = (char *)(proto != NULL ? proto : "tcp");
    return &service;
}

int WSAAPI WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    if (lpWSAData != NULL) {
        memset(lpWSAData, 0, sizeof(*lpWSAData));
        lpWSAData->wVersion = wVersionRequested;
        lpWSAData->wHighVersion = MAKEWORD(2, 2);
    }

    wsaLastError = 0;
    return 0;
}

int WSAAPI WSACleanup(void)
{
    wsaLastError = 0;
    return 0;
}

int WSAAPI WSAGetLastError(void)
{
    if (wsaLastError != 0) {
        return wsaLastError;
    }

    return nxdk_wsa_error_from_errno(errno);
}

void WSAAPI WSASetLastError(int iError)
{
    wsaLastError = iError;
}

SOCKET WSAAPI WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags)
{
    (void)lpProtocolInfo;
    (void)g;
    (void)dwFlags;

    SOCKET socketHandle = socket(af, type, protocol);
    if (socketHandle == INVALID_SOCKET) {
        nxdk_wsa_set_last_error_from_errno();
    }
    else {
        wsaLastError = 0;
    }

    return socketHandle;
}

int WSAAPI WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer,
                    LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned,
                    LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    (void)s;
    (void)lpOverlapped;
    (void)lpCompletionRoutine;

    if (lpcbBytesReturned != NULL) {
        *lpcbBytesReturned = 0;
    }

    if (dwIoControlCode == SIO_GET_EXTENSION_FUNCTION_POINTER) {
        const GUID *requestedGuid;
        LPVOID functionPointer;

        if (lpvInBuffer == NULL || cbInBuffer < sizeof(GUID) || lpvOutBuffer == NULL || cbOutBuffer < sizeof(LPVOID)) {
            WSASetLastError(WSAEFAULT);
            return SOCKET_ERROR;
        }

        requestedGuid = (const GUID *)lpvInBuffer;
        if (nxdk_guid_equal(requestedGuid, &WSAID_WSARECVMSG)) {
            functionPointer = (LPVOID)WSARecvMsg;
        }
        else if (nxdk_guid_equal(requestedGuid, &WSAID_WSASENDMSG)) {
            functionPointer = (LPVOID)WSASendMsg;
        }
        else {
            WSASetLastError(WSAEOPNOTSUPP);
            return SOCKET_ERROR;
        }

        memcpy(lpvOutBuffer, &functionPointer, sizeof(functionPointer));
        if (lpcbBytesReturned != NULL) {
            *lpcbBytesReturned = sizeof(functionPointer);
        }
        WSASetLastError(0);
        return 0;
    }

    if (dwIoControlCode == SIO_UDP_CONNRESET) {
        WSASetLastError(0);
        return 0;
    }

    WSASetLastError(WSAEOPNOTSUPP);
    return SOCKET_ERROR;
}

INT WSAAPI WSARecvMsg(SOCKET s, LPWSAMSG lpMsg, LPDWORD lpdwNumberOfBytesRecvd,
                      LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    struct msghdr message;
    ssize_t bytesReceived;

    if (lpMsg == NULL) {
        WSASetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }
    if (lpOverlapped != NULL || lpCompletionRoutine != NULL) {
        WSASetLastError(WSA_IO_PENDING);
        return SOCKET_ERROR;
    }

    nxdk_wsamsg_to_msghdr(lpMsg, &message);
    bytesReceived = recvmsg(s, &message, 0);
    if (bytesReceived < 0) {
        nxdk_wsa_set_last_error_from_errno();
        return SOCKET_ERROR;
    }

    nxdk_msghdr_to_wsamsg(&message, lpMsg);
    if (lpdwNumberOfBytesRecvd != NULL) {
        *lpdwNumberOfBytesRecvd = (DWORD)bytesReceived;
    }
    WSASetLastError(0);
    return 0;
}

INT WSAAPI WSASendMsg(SOCKET s, LPWSAMSG lpMsg, DWORD dwFlags, LPDWORD lpNumberOfBytesSent,
                      LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    struct msghdr message;
    ssize_t bytesSent;

    if (lpMsg == NULL) {
        WSASetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }
    if (lpOverlapped != NULL || lpCompletionRoutine != NULL) {
        WSASetLastError(WSA_IO_PENDING);
        return SOCKET_ERROR;
    }

    nxdk_wsamsg_to_msghdr(lpMsg, &message);
    bytesSent = sendmsg(s, &message, (int)dwFlags);
    if (bytesSent < 0) {
        nxdk_wsa_set_last_error_from_errno();
        return SOCKET_ERROR;
    }

    if (lpNumberOfBytesSent != NULL) {
        *lpNumberOfBytesSent = (DWORD)bytesSent;
    }
    WSASetLastError(0);
    return 0;
}
