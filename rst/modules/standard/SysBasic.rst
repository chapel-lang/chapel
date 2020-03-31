.. default-domain:: chpl

.. module:: SysBasic
   :synopsis: Types for low-level programming and C integration.

SysBasic
========
**Usage**

.. code-block:: chapel

   use SysBasic;

Types for low-level programming and C integration.

This module provides basic types when those types are not
platform-dependent. The module :mod:`SysCTypes` contains the
platform-dependent declarations for C compatibility.

This module also defines the error types :type:`syserr` and :type:`err_t`.

When should one use :type:`syserr` and when should one use :type:`err_t`?
:type:`err_t` is a system error code (a `c_int` by a nicer name to
indicate its purpose). :type:`syserr` is an enhanced error that might also
include an error message. All user-facing Chapel library code, or user
Chapel code, should generally use :type:`syserr`. When wrapping functions
or declaring them in C, use :type:`err_t` to indicate that a function is
returning an error code.

A note about the error code documentation in this file. Error descriptions
for system errors are included here for convenience only. Other
documentation, such as system man pages or the POSIX specification are the
authority on system error codes.


.. type:: type c_float = real(32)

   The type corresponding to a C float 

.. type:: type c_double = real(64)

   The type corresponding to a C double 

.. type:: type off_t = int(64)

   The type corresponding to C's off_t 

.. type:: type mode_t = uint(32)

   The type corresponding to C's mode_t 

.. type:: type socklen_t = int(32)

   The type corresponding to C's socklen_t 

.. type:: type syserr

   A type storing an error code or an error message.
   A syserr can be compared using == or != to an err_t (ie integer error code)
   or to another syserr. A syserr can be cast to or from an err_t. It can be
   assigned the value of an err_t or another syserr. In addition, syserr can be
   checked directly in an if statement like so:
   
   .. code-block:: chapel
   
     var err: syserr;
     if err then do writeln("err contains an error, ie err != ENOERR");
     if !err then do writeln("err does not contain an error; err == ENOERR");
   
   
   When a :type:`syserr` formal has default intent, the actual is copied to the
   formal upon a function call and the formal cannot be assigned within the
   function.
   
   The default value of the :type:`syserr` type is undefined.
   
   

.. type:: type err_t = c_int

   An integral error code. This is really just a `c_int`, but code is
   clearer if you use err_t to indicate arguments, variables, and return types
   that are system error codes. 
   

.. type:: type fd_t = c_int

   A system file descriptor. This is really just a `c_int`, but code is
   clearer if you use fd_t to indicate arguments, variables, and return types
   that are system file descriptors.
   

.. function:: proc ENOERR

   The error code indicating that no error occurred (Chapel specific) 

.. function:: proc EEOF

   An error code indicating the end of file has been reached (Chapel specific)
   

.. function:: proc ESHORT

   An error code indicating that the end of file or the end of the
   input was reached before the requested amount of data could be read.
   (Chapel specific)
   

.. function:: proc EFORMAT

   An error code indicating a format error; for example when reading a quoted
   string literal, this would be returned if we never encountered the
   opening quote. (Chapel specific)
   

.. data:: const E2BIG: err_t

   Argument list too long. The number of bytes used for the argument and
   environment list of the new process exceeded the current limit. (POSIX.1)
   

.. data:: const EACCES: err_t

   Permission denied. An attempt was made to access a file in a way forbidden
   by its file access permissions. (POSIX.1)
   

.. data:: const EADDRINUSE: err_t

   Address already in use. Only one usage of each address is normally permitted.
   (POSIX.1)
   

.. data:: const EADDRNOTAVAIL: err_t

   Can't assign requested address. Normally results from an attempt to create
   a socket with an address not on this machine. (POSIX.1)
   

.. data:: const EAFNOSUPPORT: err_t

   Address family not supported by protocol family. An address incompatible
   with the requested protocol was used. For example, you should not
   necessarily expect to be able to use NS addresses with ARPA Internet
   protocols. (POSIX.1)

.. data:: const EAGAIN: err_t

   Resource temporarily unavailable. This is a temporary condition and later
   calls to the same routine may complete normally. (POSIX.1)
   

.. data:: const EALREADY: err_t

   Operation already in progress. An operation was attempted on a non-blocking
   object that already had an operation in progress. (POSIX.1)
   

.. data:: const EBADE: err_t

   Invalid exchange (linux only) 

.. data:: const EBADF: err_t

   Bad file descriptor. A file descriptor argument was out of range, referred
   to no open file, or a read (write) request was made to a file that was only
   open for writing (reading). (POSIX.1)
   

.. data:: const EBADFD: err_t

   File descriptor in bad state (linux only) 

.. data:: const EBADMSG: err_t

   Bad message. A corrupted message was detected. (POSIX.1) 

.. data:: const EBADR: err_t

   Invalid request descriptor (linux only) 

.. data:: const EBADRQC: err_t

   Invalid request code (linux only) 

.. data:: const EBADSLT: err_t

   Invalid slot (linux only) 

.. data:: const EBUSY: err_t

   Device or resource busy. An attempt to use a system resource which was in
   use at the time in a manner which would have conflicted with the request.
   (POSIX.1)
   

.. data:: const ECANCELED: err_t

   Operation canceled. The scheduled operation was canceled. (POSIX.1) 

.. data:: const ECHILD: err_t

   No child processes. A wait or waitpid system call was executed by a
   process that had no existing or unwaited-for child processes. (POSIX.1)
   

.. data:: const ECHRNG: err_t

   Channel number out of range (linux only) 

.. data:: const ECOMM: err_t

   Communication error on send (linux only) 

.. data:: const ECONNABORTED: err_t

   Software caused connection abort. A connection abort was caused internal
   to your host machine. (POSIX.1)
   

.. data:: const ECONNREFUSED: err_t

   Connection refused. No connection could be made because the target machine
   actively refused it. This usually results from trying to connect to a
   service that is inactive on the foreign host. (POSIX.1)
   

.. data:: const ECONNRESET: err_t

   Connection reset by peer. A connection was forcibly closed by a peer. This
   normally results from a loss of the connection on the remote socket due to a
   timeout or a reboot. (POSIX.1) 
   

.. data:: const EDEADLK: err_t

   Resource deadlock avoided. An attempt was made to lock a system resource
   that would have resulted in a deadlock situation. (POSIX.1)
   

.. data:: const EDESTADDRREQ: err_t

   Destination address required. A required address was omitted from an
   operation on a socket. (POSIX.1)
   

.. data:: const EDOM: err_t

   Numerical argument out of domain. A numerical input argument was outside
   the defined domain of the mathematical function. (POSIX.1, C99)
   

.. data:: const EDQUOT: err_t

   Disc quota exceeded. A write system call to an ordinary file, the creation
   of a directory or symbolic link, or the creation of a directory entry failed
   because the user's quota of disk blocks was exhausted, or the allocation of
   an inode for a newly created file failed because the user's quota of inodes
   was exhausted. (POSIX.1)
   

.. data:: const EEXIST: err_t

   File exists. An existing file was mentioned in an inappropriate context, for
   instance, as the new link name in a link system call. (POSIX.1)
   

.. data:: const EFAULT: err_t

   Bad address. The system detected an invalid address in attempting to
   use an argument of a call. (POSIX.1)
   

.. data:: const EFBIG: err_t

   File too large. The size of a file exceeded the maximum. (POSIX.1) 

.. data:: const EHOSTDOWN: err_t

   Host is down. A socket operation failed because the destination host was
   down. (linux, FreeBSD)
   

.. data:: const EHOSTUNREACH: err_t

   No route to host. A socket operation was attempted to an unreachable host.
   (POSIX.1)
   

.. data:: const EIDRM: err_t

   Identifier removed. An IPC identifier was removed while the current
   process was waiting on it. (POSIX.1)
   

.. data:: const EILSEQ: err_t

   
   Illegal byte sequence. While decoding a multibyte character the function
   came along an invalid or an incomplete sequence of bytes or the given wide
   character is invalid.
   
   This error might be returned for example in the case of an illegal UTF-8
   byte sequence. (POSIX.1, C99)
   

.. data:: const EINPROGRESS: err_t

   Operation now in progress. An operation that takes a long time to complete
   (such as a connect system call) was attempted on a non-blocking object.
   (POSIX.1)
   

.. data:: const EINTR: err_t

   Interrupted system call. An asynchronous signal (such as SIGINT or SIGQUIT)
   was caught by the process during the execution of an interruptible function.
   If the signal handler performs a normal return, the interrupted system call
   will seem to have returned the error condition. (POSIX.1)

.. data:: const EINVAL: err_t

   Invalid argument. Some invalid argument was supplied. (For example,
   specifying an undefined signal to a signal system call or a kill system
   call). (POSIX.1)

.. data:: const EIO: err_t

   Input/output error. Some physical input or output error occurred. This
   error will not be reported until a subsequent operation on the same file
   descriptor and may be lost (over written) by any subsequent errors. (POSIX.1)
   

.. data:: const EISCONN: err_t

   Socket is already connected. A connect system call was made on an already
   connected socket; or, a sendto or sendmsg system call on a connected socket
   specified a destination when already connected. (POSIX.1)

.. data:: const EISDIR: err_t

   Is a directory. An attempt was made to open a directory with write mode
   specified. (POSIX.1)
   

.. data:: const EISNAM: err_t

   Is a named type file (linux only) 

.. data:: const EKEYEXPIRED: err_t

   Key has expired (linux only) 

.. data:: const EKEYREJECTED: err_t

   Key was rejected by service (linux only) 

.. data:: const EKEYREVOKED: err_t

   Key has been revoked (linux only) 

.. data:: const EL2HLT: err_t

   Level 2 halted (linux only) 

.. data:: const EL2NSYNC: err_t

   Level 2 not synchronized (linux only) 

.. data:: const EL3HLT: err_t

   Level 3 halted (linux only) 

.. data:: const EL3RST: err_t

   Level 3 halted (linux only) 

.. data:: const ELIBACC: err_t

   Cannot access a needed shared library (linux only) 

.. data:: const ELIBBAD: err_t

   Accessing a corrupted shared library (linux only) 

.. data:: const ELIBMAX: err_t

   Attempting to link in too many shared libraries (linux only) 

.. data:: const ELIBSCN: err_t

   lib section in a.out corrupted (linux only) 

.. data:: const ELIBEXEC: err_t

   Cannot exec a shared library directly (linux only) 

.. data:: const ELOOP: err_t

   Too many levels of symbolic links. A path name lookup involved more than 32
   (MAXSYMLINKS) symbolic links. (POSIX.1)
   

.. data:: const EMEDIUMTYPE: err_t

   Wrong medium type (linux only) 

.. data:: const EMFILE: err_t

   Too many open files. Maximum number of file descriptors allowable in the
   process has been reached and requests for an open cannot be satisfied until
   at least one has been closed. The getdtablesize system call will obtain the
   current limit. (POSIX.1)

.. data:: const EMLINK: err_t

   Too many links. Maximum allowable hard links to a single file has been
   exceeded. (POSIX.1)
   

.. data:: const EMSGSIZE: err_t

   Message too long. A message sent on a socket was larger than the internal 
   message buffer or some other network limit. (POSIX.1)
   

.. data:: const EMULTIHOP: err_t

   Multihop attempted. (POSIX.1)
   

.. data:: const ENAMETOOLONG: err_t

   File name too long. A component of a path name exceeded {NAME_MAX}
   characters, or an entire path name exceeded {PATH_MAX} characters. (POSIX.1)
   

.. data:: const ENETDOWN: err_t

   Network is down. A socket operation encountered a dead network. (POSIX.1) 

.. data:: const ENETRESET: err_t

   Network dropped connection on reset. The host you were connected to crashed
   and rebooted. (POSIX.1)
   

.. data:: const ENETUNREACH: err_t

   Network is unreachable. A socket operation was attempted to an unreachable
   network. (POSIX.1)
   

.. data:: const ENFILE: err_t

   Too many open files in system. Maximum number of open files allowable on 
   the system has been reached and requests for an open cannot be satisfied
   until at least one has been closed. (POSIX.1)
   

.. data:: const ENOBUFS: err_t

   No buffer space available. An operation on a socket or pipe was not
   performed because the system lacked sufficient buffer space or because a
   queue was full. (POSIX.1 XSI STREAMS option)
   

.. data:: const ENODATA: err_t

   No  message is available on the STREAM head read queue (POSIX.1)
   

.. data:: const ENODEV: err_t

   Operation not supported by device. An attempt was made to apply an
   inappropriate function to a device, for example, trying to read a write-only
   device such as a printer. (POSIX.1)
   

.. data:: const ENOENT: err_t

   No such file or directory. A component of a specified pathname did not
   exist, or the pathname was an empty string. (POSIX.1)
   

.. data:: const ENOEXEC: err_t

   Exec format error. A request was made to execute a file that, although it
   has the appropriate permissions, was not in the format required for an
   executable file. (POSIX.1)
   

.. data:: const ENOKEY: err_t

   Required key not available (linux only) 

.. data:: const ENOLCK: err_t

   No locks available. A system-imposed limit on the number of simultaneous 
   file locks was reached. (POSIX.1)
   

.. data:: const ENOLINK: err_t

   Link has been severed. (POSIX.1)
   

.. data:: const ENOMEDIUM: err_t

   No medium found (linux only) 

.. data:: const ENOMEM: err_t

   Cannot allocate memory. The new process image required more memory than
   was allowed by the hardware or by system-imposed memory management
   constraints. A lack of swap space is normally temporary; however, a lack of
   core is not. Soft limits may be increased to their corresponding hard
   limits. (POSIX.1)
   

.. data:: const ENOMSG: err_t

   No message of desired type. An IPC message queue does not contain a message
   of the desired type, or a message catalog does not contain the requested
   message. (POSIX.1)
   

.. data:: const ENONET: err_t

   Machine is not on the network (linux only)
   

.. data:: const ENOPKG: err_t

   Package not installed  (linux only)
   

.. data:: const ENOPROTOOPT: err_t

   Protocol not available. A bad option or level was specified in a
   getsockopt or setsockopt system call. (POSIX.1)
   

.. data:: const ENOSPC: err_t

   No space left on device. A write system call to an ordinary file, the
   creation of a directory or symbolic link, or the creation of a directory
   entry failed because no more disk blocks were available on the file system,
   or the allocation of an inode for a newly created file failed because no
   more inodes were available on the file system. (POSIX.1)
   

.. data:: const ENOSR: err_t

   No STREAM resources (POSIX.1 XSI STREAMS option) 

.. data:: const ENOSTR: err_t

   Not a STREAM (POSIX.1 XSI STREAMS option) 

.. data:: const ENOSYS: err_t

   Function not implemented. Attempted a system call that is not available on 
   this system. (POSIX.1)
   

.. data:: const ENOTBLK: err_t

   Block device required.
   A block device operation was attempted on a non-block device or file.
   (linux, FreeBSD)
   

.. data:: const ENOTCONN: err_t

   Socket is not connected. An request to send or receive data was disallowed
   because the socket was not connected and (when sending on a datagram socket)
   no address was supplied. (POSIX.1)
   

.. data:: const ENOTDIR: err_t

   Not a directory. A component of the specified pathname existed, but it was
   not a directory, when a directory was expected. (POSIX.1)
   

.. data:: const ENOTEMPTY: err_t

   Directory not empty. A directory with entries other than '.' and '..' was 
   supplied to a remove directory or rename call. (POSIX.1)
   

.. data:: const ENOTSOCK: err_t

   Socket operation on non-socket. (POSIX.1) 

.. data:: const ENOTSUP: err_t

   Operation not supported. The attempted operation is not supported for the
   type of object referenced. Usually this occurs when a file descriptor
   refers to a file or socket that cannot support this operation, for example,
   trying to accept a connection on a datagram socket. (POSIX.1)
   

.. data:: const ENOTTY: err_t

   Inappropriate ioctl for device. A control function (e.g. ioctl system
   call) was attempted for a file or special device for which the operation was
   inappropriate. (POSIX.1)
   

.. data:: const ENOTUNIQ: err_t

   Name not unique on network (linux only) 

.. data:: const ENXIO: err_t

   Device not configured. Input or output on a special file referred to a
   device that did not exist, or made a request beyond the limits of the
   device. This error may also occur when, for example, a tape drive is
   not online or no disk pack is loaded on a drive. (POSIX.1)
   

.. data:: const EOPNOTSUPP: err_t

   Operation not supported. The attempted operation is not supported for the
   type of object referenced. Usually this occurs when a file descriptor refers
   to a file or socket that cannot support this operation, for example, trying
   to accept a connection on a datagram socket. (POSIX.1)
   

.. data:: const EOVERFLOW: err_t

   Value too large to be stored in data type. A numerical result of the
   function was too large to be stored in the caller provided space. (POSIX.1)
   

.. data:: const EPERM: err_t

   Operation not permitted. An attempt was made to perform an operation
   limited to processes with appropriate privileges or to the owner of a file
   or other resources. (POSIX.1)
   

.. data:: const EPFNOSUPPORT: err_t

   Protocol family not supported. The protocol family has not been configured
   into the system or no implementation for it exists. (linux, FreeBSD)
   

.. data:: const EPIPE: err_t

   Broken pipe. A write on a pipe, socket or FIFO for which there is no 
   process to read the data. (POSIX.1)
   

.. data:: const EPROTO: err_t

   Protocol error. A device or socket encountered an unrecoverable 
   protocol error. (POSIX.1)
   

.. data:: const EPROTONOSUPPORT: err_t

   Protocol not supported. The protocol has not been configured into the
   system or no implementation for it exists. (POSIX.1)
   

.. data:: const EPROTOTYPE: err_t

   Protocol wrong type for socket. A protocol was specified that does not sup-
   port the semantics of the socket type requested. For example, you cannot
   use the ARPA Internet UDP protocol with type SOCK_STREAM. (POSIX.1)
   

.. data:: const ERANGE: err_t

   Result too large. A numerical result of the function was too large to fit 
   in the available space (perhaps exceeded precision). (POSIX.1, C99)
   

.. data:: const EREMCHG: err_t

   Remote address changed (linux only) 

.. data:: const EREMOTE: err_t

   Object is remote (linux only) 

.. data:: const EREMOTEIO: err_t

   Remote I/O error (linux only) 

.. data:: const ERESTART: err_t

   Interrupted system call should be restarted (linux only) 

.. data:: const EROFS: err_t

   Read-only file system. An attempt was made to modify a file or directory on
   a file system that was read-only at the time. (POSIX.1)
   

.. data:: const ESHUTDOWN: err_t

   Can't send after socket shutdown. A request to send data was disallowed
   because the socket had already been shut down with a previous shutdown system
   call.
   

.. data:: const ESPIPE: err_t

   Illegal seek. An lseek system call was issued on a socket, pipe or FIFO. 
   (POSIX.1)
   

.. data:: const ESOCKTNOSUPPORT: err_t

   Socket type not supported. The support for the socket type has not been
   configured into the system or no implementation for it exists. 
   (linux, FreeBSD)
   

.. data:: const ESRCH: err_t

   No such process. No process could be found corresponding to that specified 
   by the given process ID. (POSIX.1)
   

.. data:: const ESTALE: err_t

   Stale NFS file handle. An attempt was made to access an open file (on an 
   NFS file system) which is now unavailable as referenced by the file
   descriptor. This may indicate the file was deleted on the NFS server or
   some other catastrophic event occurred. (POSIX.1)
   

.. data:: const ESTRPIPE: err_t

   Streams pipe error (linux only) 

.. data:: const ETIME: err_t

   Timer expired (POSIX.1 XSI STREAMS option)
   

.. data:: const ETIMEDOUT: err_t

   Operation timed out. A connect or send system call failed because the 
   connected party did not properly respond after a period of time. (The
   timeout period is dependent on the communication protocol.) (POSIX.1)
   

.. data:: const ETXTBSY: err_t

   Text file busy. The new process was a pure procedure (shared text) file 
   which was open for writing by another process, or while the pure procedure
   file was being executed an open system call requested write access. (POSIX.1)
   

.. data:: const EUCLEAN: err_t

   Structure needs cleaning (linux only) 

.. data:: const EUNATCH: err_t

   Protocol driver not attached (linux only) 

.. data:: const EUSERS: err_t

   Too many users. The quota system ran out of table entries. 
   (linux, FreeBSD)
   

.. data:: const EWOULDBLOCK: err_t

   Operation would block (may be same value as EAGAIN) (POSIX.1)
   

.. data:: const EXDEV: err_t

   Cross-device link. A hard link to a file on another file system was 
   attempted. (POSIX.1)
   

.. data:: const EXFULL: err_t

   Exchange full (linux only) 

