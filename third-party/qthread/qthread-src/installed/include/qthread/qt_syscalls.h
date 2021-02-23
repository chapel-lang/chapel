#ifndef QT_SYSCALLS_H
#define QT_SYSCALLS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>   /* for fd_set */
#include <sys/resource.h> /* for struct rusage */
#include <poll.h>         /* for struct pollfd and nfds_t */

#include "macros.h"

Q_STARTCXX /* */

int qt_accept(int                       socket,
              struct sockaddr *restrict address,
              socklen_t *restrict       address_len);
int qt_connect(int                    socket,
               const struct sockaddr *address,
               socklen_t              address_len);
int qt_poll(struct pollfd fds[],
            nfds_t        nfds,
            int           timeout);
ssize_t qt_pread(int    filedes,
                 void  *buf,
                 size_t nbyte,
                 off_t  offset);
ssize_t qt_pwrite(int         filedes,
                  const void *buf,
                  size_t      nbyte,
                  off_t       offset);
ssize_t qt_read(int    filedes,
                void  *buf,
                size_t nbyte);
int qt_select(int                      nfds,
              fd_set *restrict         readfds,
              fd_set *restrict         writefds,
              fd_set *restrict         errorfds,
              struct timeval *restrict timeout);
int   qt_system(const char *command);
pid_t qt_wait4(pid_t          pid,
               int           *stat_loc,
               int            options,
               struct rusage *rusage);
ssize_t qt_write(int         filedes,
                 const void *buf,
                 size_t      nbyte);

#ifdef USE_HEADER_SYSCALLS
# define accept(s, a, l)       qt_accept((s), (a), (l))
# define connect(s, a, l)      qt_connect((s), (a), (l))
# define poll(f, n, t)         qt_poll((f), (n), (t))
# define pread(f, b, n, o)     qt_pread((f), (b), (n), (o))
# define pwrite(f, b, n, o)    qt_pwrite((f), (b), (n), (o))
# define read(f, b, n)         qt_read((f), (b), (n))
# define select(n, r, w, e, t) qt_select((n), (r), (w), (e), (t))
# define system(c)             qt_system((c))
# define wait4(p, s, o, r)     qt_wait4((p), (s), (o), (r))
# define write(f, b, n)        qt_write((f), (b), (n))
#endif // ifdef USE_HEADER_SYSCALLS

Q_ENDCXX /* */

#endif // ifndef QT_SYSCALLS_H
/* vim:set expandtab: */
