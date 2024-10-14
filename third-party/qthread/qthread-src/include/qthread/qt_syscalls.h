#ifndef QT_SYSCALLS_H
#define QT_SYSCALLS_H

#include <poll.h>         /* for struct pollfd and nfds_t */
#include <sys/resource.h> /* for struct rusage */
#include <sys/select.h>   /* for fd_set */
#include <sys/socket.h>
#include <sys/types.h>

#include "macros.h"

Q_STARTCXX /* */

int qt_accept(int                       socket,
              struct sockaddr *restrict address,
              socklen_t *restrict       address_len);
int qt_connect(int socket,
               const struct sockaddr *address,
               socklen_t address_len);
int qt_poll(struct pollfd fds[], nfds_t nfds, int timeout);
ssize_t qt_pread(int filedes, void *buf, size_t nbyte, off_t offset);
ssize_t qt_pwrite(int filedes, void const *buf, size_t nbyte, off_t offset);
ssize_t qt_read(int filedes, void *buf, size_t nbyte);
int qt_select(int nfds,
              fd_set *restrict readfds,
              fd_set *restrict writefds,
              fd_set *restrict errorfds,
              struct timeval *restrict timeout);
int qt_system(char const *command);
pid_t qt_wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);
ssize_t qt_write(int filedes, void const *buf, size_t nbyte);

Q_ENDCXX /* */

#endif // ifndef QT_SYSCALLS_H
  /* vim:set expandtab: */
