/* 
 * myth_wrap_socket.c : socket
 */

#include "myth_real.h"
#include "myth_wrap_util_func.h"

int __wrap(socket)(int domain, int type, int protocol) {
  int _ = enter_wrapped_func("%d, %d, %d", domain, type, protocol);
  int x = real_socket(domain, type, protocol);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(socketpair)(int domain, int type, int protocol, int sv[2]) {
  int _ = enter_wrapped_func("%d, %d, %d, %p", domain, type, protocol, sv);
  int x = real_socketpair(domain, type, protocol, sv);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(accept)(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
  int _ = enter_wrapped_func("%d, %p, %p", sockfd, addr, addrlen);
  int x = real_accept(sockfd, addr, addrlen);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

#if defined(HAVE_ACCEPT4)
#if _GNU_SOURCE
int __wrap(accept4)(int sockfd, struct sockaddr *addr,
		    socklen_t *addrlen, int flags) {
  int _ = enter_wrapped_func("%d, %p, %p, %d", sockfd, addr, addrlen, flags);
  int x = real_accept4(sockfd, addr, addrlen, flags);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}
#endif	/* _GNU_SOURCE */
#endif	/* HAVE_ACCEPT4 */


int __wrap(bind)(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int _ = enter_wrapped_func("%d, %p, %lu", sockfd, addr, addrlen);
  int x = real_bind(sockfd, addr, addrlen);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(close)(int fd) {
  int _ = enter_wrapped_func("%d", fd);
  int x = real_close(fd);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(connect)(int sockfd, const struct sockaddr *addr,
		    socklen_t addrlen) {
  int _ = enter_wrapped_func("%d, %p, %lu", sockfd, addr, addrlen);
  int x = real_connect(sockfd, addr, addrlen);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(fcntl)(int fd, int cmd, ... /* arg */ ) {
  switch (cmd) {
    /* cmd not taking any arg */
  case F_GETFD:
  case F_GETFL:
  case F_GETOWN:
  case F_GETSIG:
  case F_GETLEASE:
#if defined(F_GETPIPE_SZ)
  case F_GETPIPE_SZ:		/*  (void; since Linux 2.6.35) */
#endif
#if defined(F_ADD_SEALS)
  case F_GET_SEALS:		/*  (void; since Linux 3.17) */
#endif
    return real_fcntl(fd, cmd);
    /* cmd taking int */
  case F_DUPFD:
#if defined(F_DUPFD_CLOEXEC)
  case F_DUPFD_CLOEXEC:		/* (int; since Linux 2.6.24) */
#endif
  case F_SETFD:
  case F_SETFL:
  case F_SETOWN:
  case F_SETSIG:
  case F_SETLEASE:
  case F_NOTIFY:
#if defined(F_SETPIPE_SZ)
  case F_SETPIPE_SZ:		/*  (int; since Linux 2.6.35) */
#endif
#if defined(F_ADD_SEALS)
  case F_ADD_SEALS:		/*  (int; since Linux 3.17) */
#endif
    /* cmd taking an int */
    {
      va_list ap;
      va_start(ap, cmd);
      int arg = va_arg(ap, int);
      va_end(ap);
      return real_fcntl(fd, cmd, arg);
    }
  case F_SETLK:
  case F_SETLKW:
  case F_GETLK:
#if defined(F_OFD_SETLK)
  case F_OFD_SETLK:
#endif
#if defined(F_OFD_SETLKW)
  case F_OFD_SETLKW:
#endif
#if defined(F_OFD_GETLK)
  case F_OFD_GETLK:
#endif
#if defined(F_GETOWN_EX)
  case F_GETOWN_EX:	 /* (since Linux 2.6.32) */
#endif
#if defined(F_SETOWN_EX)
  case F_SETOWN_EX: /* (since Linux 2.6.32)  */
#endif
    /* cmd taking a pointer */
    {
      va_list ap;
      va_start(ap, cmd);
      void * arg = va_arg(ap, void*);
      va_end(ap);
      return real_fcntl(fd, cmd, arg);
    }
  default:
    /* punt; assume int-taking */
    /* cmd taking a pointer */
    {
      va_list ap;
      va_start(ap, cmd);
      void * arg = va_arg(ap, void*);
      va_end(ap);
      return real_fcntl(fd, cmd, arg);
    }
  }
}

int __wrap(listen)(int sockfd, int backlog) {
  int _ = enter_wrapped_func("%d, %d", sockfd, backlog);
  int x = real_listen(sockfd, backlog);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(recv)(int sockfd, void *buf, size_t len, int flags) {
  int _ = enter_wrapped_func("%d, %p, %lu, %d", sockfd, buf, len, flags);
  ssize_t x = real_recv(sockfd, buf, len, flags);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(recvfrom)(int sockfd, void *buf, size_t len, int flags,
			 struct sockaddr *src_addr, socklen_t *addrlen) {
  int _ = enter_wrapped_func("%d, %p, %lu, %d, %p, %p",
			     sockfd, buf, len, flags, src_addr, addrlen);
  ssize_t x = real_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(recvmsg)(int sockfd, struct msghdr *msg, int flags) {
  int _ = enter_wrapped_func("%d, %p, %d", sockfd, msg, flags);
  ssize_t x = real_recvmsg(sockfd, msg, flags);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(read)(int fd, void *buf, size_t count) {
  int _ = enter_wrapped_func("%d, %p, %lu", fd, buf, count);
  ssize_t x = real_read(fd, buf, count);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(select)(int nfds, fd_set *readfds, fd_set *writefds,
		   fd_set *exceptfds, struct timeval *timeout) {
  int _ = enter_wrapped_func("%d, %p, %p, %p, %p", nfds, readfds, writefds, exceptfds, timeout);
  int x = real_select(nfds, readfds, writefds, exceptfds, timeout);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(send)(int sockfd, const void *buf, size_t len, int flags) {
  int _ = enter_wrapped_func("%d, %p, %lu, %d", sockfd, buf, len, flags);
  ssize_t x = real_send(sockfd, buf, len, flags);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(sendto)(int sockfd, const void *buf, size_t len, int flags,
		       const struct sockaddr *dest_addr, socklen_t addrlen) {
  int _ = enter_wrapped_func("%d, %p, %lu, %d, %p, %p",
			     sockfd, buf, len, flags, dest_addr, addrlen);
  ssize_t x = real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(sendmsg)(int sockfd, const struct msghdr *msg, int flags) {
  int _ = enter_wrapped_func("%d, %p, %d", sockfd, msg, flags);
  ssize_t x = real_sendmsg(sockfd, msg, flags);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

ssize_t __wrap(write)(int fd, const void *buf, size_t count) {
  int _ = enter_wrapped_func("%d, %p, %lu", fd, buf, count);
  ssize_t x = real_write(fd, buf, count);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}
