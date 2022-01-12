---
layout: page
title: fi_errno(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_errno \- fabric errors

fi_strerror \- Convert fabric error into a printable string

# SYNOPSIS

```c
#include <rdma/fi_errno.h>

const char *fi_strerror(int errno);
```


# ERRORS

*FI_ENOENT*
: No such file or directory.

*FI_EIO*
: I/O error

*FI_E2BIG*
: Argument list too long.

*FI_EBADF*
: Bad file number.

*FI_EAGAIN*
: Try again.

*FI_ENOMEM*
: Out of memory.

*FI_EACCES*
: Permission denied.

*FI_EBUSY*
: Device or resource busy

*FI_ENODEV*
: No such device

*FI_EINVAL*
: Invalid argument

*FI_EMFILE*
: Too many open files

*FI_ENOSPC*
: No space left on device

*FI_ENOSYS*
: Function not implemented

*FI_ENOMSG*
: No message of desired type

*FI_ENODATA*
: No data available

*FI_EMSGSIZE*
: Message too long

*FI_ENOPROTOOPT*
: Protocol not available

*FI_EOPNOTSUPP*
: Operation not supported on transport endpoint

*FI_EADDRINUSE*
: Address already in use

*FI_EADDRNOTAVAIL*
: Cannot assign requested address

*FI_ENETDOWN*
: Network is down

*FI_ENETUNREACH*
: Network is unreachable

*FI_ECONNABORTED*
: Software caused connection abort

*FI_ECONNRESET*
: Connection reset by peer

*FI_EISCONN*
: Transport endpoint is already connected

*FI_ENOTCONN*
: Transport endpoint is not connected

*FI_ESHUTDOWN*
: Cannot send after transport endpoint shutdown

*FI_ETIMEDOUT*
: Operation timed out

*FI_ECONNREFUSED*
: Connection refused

*FI_EHOSTUNREACH*
: No route to host

*FI_EALREADY*
: Operation already in progress

*FI_EINPROGRESS*
: Operation now in progress

*FI_EREMOTEIO*
: Remote I/O error

*FI_ECANCELED*
: Operation Canceled

*FI_ENOKEY*
: Required key not available

*FI_EKEYREJECTED*
: Key was rejected by service

*FI_EOTHER*
: Unspecified error

*FI_ETOOSMALL*
: Provided buffer is too small

*FI_EOPBADSTATE*
: Operation not permitted in current state

*FI_EAVAIL*
: Error available

*FI_EBADFLAGS*
: Flags not supported

*FI_ENOEQ*
: Missing or unavailable event queue

*FI_EDOMAIN*
: Invalid resource domain

*FI_ENOCQ*
: Missing or unavailable completion queue

# SEE ALSO

[`fabric`(7)](fabric.7.html)
