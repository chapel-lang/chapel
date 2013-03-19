/*   $Source: /var/local/cvs/gasnet/other/amudp/amudp_cdefs.c,v $
 *     $Date: 2005/08/19 04:37:37 $
 * $Revision: 1.2 $
 * Description: AMUDP definitions that must be compiled in C mode
 * Copyright 2005, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <stddef.h>
#include <amudp.h>
#include <socket.h>

#if AMUDP_DEBUG
  /* use the gasnet debug malloc functions if a debug libgasnet is linked
   * these *must* be tentative definitions for this linker trick to work, 
   * and C++ annoying provides apparently no way to express that
   */
  void *(*gasnett_debug_malloc_fn)(size_t sz, const char *curloc);
  void *(*gasnett_debug_calloc_fn)(size_t N, size_t S, const char *curloc);
  void (*gasnett_debug_free_fn)(void *ptr, const char *curloc);
#endif

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
