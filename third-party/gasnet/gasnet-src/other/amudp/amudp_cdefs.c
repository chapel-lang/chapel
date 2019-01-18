/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/amudp/amudp_cdefs.c $
 * Description: AMUDP definitions that must be compiled in C mode
 * Copyright 2005, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include "amudp_internal.h"

#if SOCK_USE_C_BYPASS
  /* system calls which differ in signature across OS's in ways that C++ cannot deal with */
  extern int SOCK_getsockopt(int  s, int level, int optname, void *optval, GETSOCKOPT_LENGTH_T *optlen) {
    return getsockopt(s, level, optname, optval, (void *)optlen);
  }
  extern int SOCK_getsockname(int s, struct sockaddr *name, GETSOCKNAME_LENGTH_T *namelen) {
    return getsockname(s, name, (void *)namelen);
  }
  extern int SOCK_getpeername(int s, struct sockaddr *name, GETSOCKNAME_LENGTH_T *namelen) {
    return getpeername(s, name, (void *)namelen);
  }
  extern int SOCK_ioctlsocket(int d, int request, IOCTL_FIONREAD_ARG_T *val) {
    return ioctlsocket(d, request, (void *)val);
  }
  extern int SOCK_accept(SOCKET listener, struct sockaddr* calleraddr, LENGTH_PARAM *sz) {
    return accept(listener, calleraddr, (void *)sz);
  }
  extern int SOCK_recvfrom(SOCKET s, char * buf, int len, int flags, struct sockaddr *from, LENGTH_PARAM *sz) {
    return recvfrom(s, buf, len, flags, from, (void *)sz);
  }
#endif

#include "amx_internal.c"

