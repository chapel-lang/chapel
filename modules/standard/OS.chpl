/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
   The ``OS`` module provides definitions matching operating system
   interfaces.

   This module provides Chapel declarations for the constants, types,
   and functions defined by various operating systems' programmatic
   interfaces.  It is not complete for any operating system, but does
   define those things that have been needed so far in implementing
   other Chapel modules and user programs.  It is expected to grow as
   needed.  In all respects (naming, capitalization, types, semantics)
   the symbols defined here should match their corresponding operating
   system definitions to the extent Chapel can do so.  For documentation
   on these symbols, please see the operating system manual pages.

 */
module OS {
  /*
     The ``OS.POSIX`` module provides definitions matching the POSIX
     programming interface, specifically POSIX.1-2017.  That standard
     can be found at <https://pubs.opengroup.org/onlinepubs/9699919799/>.

     There is one unavoidable difference between POSIX and ``OS.POSIX``.
     POSIX defines a function named ``select()``, which ``OS.POSIX``
     could not use because ``select`` is itself a Chapel keyword.
  */
  module POSIX {
    public use CTypes;

    //
    // sys/types.h
    //
    /*
       Explicit conversions between ``blkcnt_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type blkcnt_t;
    pragma "no doc"
    inline operator :(x:blkcnt_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:blkcnt_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``blksize_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type blksize_t;
    pragma "no doc"
    inline operator :(x:blksize_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:blksize_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``dev_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type dev_t;
    pragma "no doc"
    inline operator :(x:dev_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:dev_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``gid_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type gid_t;
    pragma "no doc"
    inline operator :(x:gid_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:gid_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``ino_t`` and ``c_uint`` are
       also defined, to support usability.
    */
    extern type ino_t;
    pragma "no doc"
    inline operator :(x:ino_t, type t:c_uint)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_uint, type t:ino_t)
      return __primitive("cast", t, x);

    /*
       Bitwise-AND and bitwise-OR operators are defined on ``mode_t``
       operands, to support querying and constructing mode values.
       Explicit conversions between ``mode_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type mode_t;
    pragma "no doc"
    inline operator &(a: mode_t, b: mode_t)
      return (a:c_int & b:c_int):mode_t;
    pragma "no doc"
    inline operator |(a: mode_t, b: mode_t)
      return (a:c_int | b:c_int):mode_t;
    pragma "no doc"
    inline operator :(x:mode_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:mode_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``nlink_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type nlink_t;
    pragma "no doc"
    inline operator :(x:nlink_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:nlink_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``off_t`` and ``c_int`` and
       ``off_t`` and integral types are also defined, to support usability.
    */
    extern type off_t;
    pragma "no doc"
    inline operator :(x:off_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:off_t)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:off_t, type t:integral)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:integral, type t:off_t)
      return __primitive("cast", t, x);


    /*
       Explicit conversions between ``suseconds_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type suseconds_t;
    pragma "no doc"
    inline operator :(x:suseconds_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:suseconds_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``time_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type time_t;
    pragma "no doc"
    inline operator :(x:time_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:time_t)
      return __primitive("cast", t, x);

    /*
       Explicit conversions between ``uid_t`` and ``c_int`` are
       also defined, to support usability.
    */
    extern type uid_t;
    pragma "no doc"
    inline operator :(x:uid_t, type t:c_int)
      return __primitive("cast", t, x);
    pragma "no doc"
    inline operator :(x:c_int, type t:uid_t)
      return __primitive("cast", t, x);

    //
    // time.h (pre-decl for struct_timespec, needed in sys/stat.h)
    //
    extern "struct timespec" record struct_timespec {
      var tv_sec:time_t;  // seconds since Jan. 1, 1970
      var tv_nsec:c_long; // and nanoseconds
    }

    //
    // errno.h
    //

    // The descriptions of these were created by combining the FreeBSD
    // manual and the linux manuals here:
    // http://www.freebsd.org/cgi/man.cgi?query=errno&apropos=0&sektion=0&manpath=FreeBSD+10.1-RELEASE&arch=default&format=html
    // http://linux.die.net/man/3/errno
    // Verified that the POSIX annotations are correct with POSIX.1-2008:
    // http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
    // Note that these error descriptions are reproduced here for convenience
    // only. These descriptions aren't the authority on the matter.


    /* Argument list too long. The number of bytes used for the argument and
       environment list of the new process exceeded the current limit.
     */
    extern const E2BIG:c_int;

    /* Permission denied. An attempt was made to access a file in a way
       forbidden by its file access permissions. (POSIX.1)
     */
    extern const EACCES:c_int;

    /* Address already in use. Only one usage of each address is normally
       permitted.
     */
    extern const EADDRINUSE:c_int;

    /* Can't assign requested address. Normally results from an attempt to
       create a socket with an address not on this machine.
     */
    extern const EADDRNOTAVAIL:c_int;

    /* Address family not supported by protocol family. An address incompatible
       with the requested protocol was used. For example, you should not
       necessarily expect to be able to use NS addresses with ARPA Internet
       protocols.
     */
    extern const EAFNOSUPPORT:c_int;

    /* Resource temporarily unavailable. This is a temporary condition and
       later calls to the same routine may complete normally.
     */
    extern const EAGAIN:c_int;

    /* Operation already in progress. An operation was attempted on a
       non-blocking object that already had an operation in progress.
     */
    extern const EALREADY:c_int;

    /* Bad file descriptor. A file descriptor argument was out of range,
       referred to no open file, or a read (write) request was made to a file
       that was only open for writing (reading).
     */
    extern const EBADF:c_int;

    /* Bad message. A corrupted message was detected. (POSIX.1) */
    extern const EBADMSG:c_int;

    /* Device or resource busy. An attempt to use a system resource which was
       in use at the time in a manner which would have conflicted with the
       request.
     */
    extern const EBUSY:c_int;

    /* Operation canceled. The scheduled operation was canceled. (POSIX.1) */
    extern const ECANCELED:c_int;

    /* No child processes. A wait or waitpid system call was executed by a
       process that had no existing or unwaited-for child processes. (POSIX.1)
     */
    extern const ECHILD:c_int;

    /* Software caused connection abort. A connection abort was caused internal
       to your host machine.
     */
    extern const ECONNABORTED:c_int;

    /* Connection refused. No connection could be made because the target
       machine actively refused it. This usually results from trying to
       connect to a service that is inactive on the foreign host.
     */
    extern const ECONNREFUSED:c_int;

    /* Connection reset by peer. A connection was forcibly closed by a peer.
       This normally results from a loss of the connection on the remote
       socket due to a timeout or a reboot.
     */
    extern const ECONNRESET:c_int;

    /* Resource deadlock avoided. An attempt was made to lock a system resource
       that would have resulted in a deadlock situation. (POSIX.1)
     */
    extern const EDEADLK:c_int;

    /* Destination address required. A required address was omitted from an
       operation on a socket.
     */
    extern const EDESTADDRREQ:c_int;

    /* Numerical argument out of domain. A numerical input argument was outside
       the defined domain of the mathematical function.
     */
    extern const EDOM:c_int;

    /* Disc quota exceeded. A write system call to an ordinary file, the
       creation of a directory or symbolic link, or the creation of a directory
       entry failed because the user's quota of disk blocks was exhausted, or
       the allocation of an inode for a newly created file failed because the
       user's quota of inodes was exhausted.
     */
    extern const EDQUOT:c_int;

    /* File exists. An existing file was mentioned in an inappropriate context,
       for instance, as the new link name in a link system call.
     */
    extern const EEXIST:c_int;

    /* Bad address. The system detected an invalid address in attempting to
       use an argument of a call.
     */
    extern const EFAULT:c_int;

    /* File too large. The size of a file exceeded the maximum. */
    extern const EFBIG:c_int;

    /* No route to host. A socket operation was attempted to an unreachable
       host.
     */
    extern const EHOSTUNREACH:c_int;

    /* Identifier removed. An IPC identifier was removed while the current
       process was waiting on it.
     */
    extern const EIDRM:c_int;

    /*
       Illegal byte sequence. While decoding a multibyte character the function
       came along an invalid or an incomplete sequence of bytes or the given
       wide character is invalid.

       This error might be returned for example in the case of an illegal UTF-8
       byte sequence.
     */
    extern const EILSEQ:c_int;

    /* Operation now in progress. An operation that takes a long time to
       complete (such as a connect system call) was attempted on a
       non-blocking object.
     */
    extern const EINPROGRESS:c_int;

    /* Interrupted system call. An asynchronous signal (such as SIGINT or
       SIGQUIT) was caught by the process during the execution of an
       interruptible function.  If the signal handler performs a normal return,
       the interrupted system call will seem to have returned the error
       condition.
     */
    extern const EINTR:c_int;

    /* Invalid argument. Some invalid argument was supplied. (For example,
       specifying an undefined signal to a signal system call or a kill system
       call).
     */
    extern const EINVAL:c_int;

   /* Input/output error. Some physical input or output error occurred. This
      error will not be reported until a subsequent operation on the same file
      descriptor and may be lost (over written) by any subsequent errors.
     */
    extern const EIO:c_int;

    /* Socket is already connected. A connect system call was made on an
       already connected socket; or, a sendto or sendmsg system call on a
       connected socket specified a destination when already connected.
     */
    extern const EISCONN:c_int;

    /* Is a directory. An attempt was made to open a directory with write mode
       specified.
     */
    extern const EISDIR:c_int;

    /* Too many levels of symbolic links. A path name lookup involved more than
       32 (MAXSYMLINKS) symbolic links.
     */
    extern const ELOOP:c_int;

    /* Too many open files. Maximum number of file descriptors
       allowable in the process has been reached and requests for an
       open cannot be satisfied until at least one has been
       closed. The getdtablesize system call will obtain the current
       limit.
     */
    extern const EMFILE:c_int;

    /* Too many links. Maximum allowable hard links to a single file has been
       exceeded.
     */
    extern const EMLINK:c_int;

    /* Message too long. A message sent on a socket was larger than the
       internal message buffer or some other network limit.
     */
    extern const EMSGSIZE:c_int;

    /* Multihop attempted.
     */
    extern const EMULTIHOP:c_int;

    /* File name too long. A component of a path name exceeded {NAME_MAX}
       characters, or an entire path name exceeded {PATH_MAX} characters.
     */
    extern const ENAMETOOLONG:c_int;

    /* Network is down. A socket operation encountered a dead network. */
    extern const ENETDOWN:c_int;

    /* Network dropped connection on reset. The host you were connected to
       crashed and rebooted.
     */
    extern const ENETRESET:c_int;

    /* Network is unreachable. A socket operation was attempted to an
       unreachable network.
     */
    extern const ENETUNREACH:c_int;

    /* Too many open files in system. Maximum number of open files allowable on
       the system has been reached and requests for an open cannot be satisfied
       until at least one has been closed.
     */
    extern const ENFILE:c_int;

    /* No buffer space available. An operation on a socket or pipe was not
       performed because the system lacked sufficient buffer space or because a
       queue was full.
     */
    extern const ENOBUFS:c_int;

    /* Operation not supported by device. An attempt was made to apply an
       inappropriate function to a device, for example, trying to read a
       write-only device such as a printer.
     */
    extern const ENODEV:c_int;

    /* No such file or directory. A component of a specified pathname did not
       exist, or the pathname was an empty string.
     */
    extern const ENOENT:c_int;

    /* Exec format error. A request was made to execute a file that, although
       it has the appropriate permissions, was not in the format required for
       an executable file.
     */
    extern const ENOEXEC:c_int;

    /* No locks available. A system-imposed limit on the number of simultaneous
      file locks was reached.
     */
    extern const ENOLCK:c_int;

    /* Link has been severed.
     */
    extern const ENOLINK:c_int;

    /* Cannot allocate memory. The new process image required more memory than
       was allowed by the hardware or by system-imposed memory management
       constraints. A lack of swap space is normally temporary; however, a lack
       of core is not. Soft limits may be increased to their corresponding hard
       limits.
     */
    extern const ENOMEM:c_int;

    /* No message of desired type. An IPC message queue does not contain a
       message of the desired type, or a message catalog does not contain the
       requested message.
     */
    extern const ENOMSG:c_int;

    /* Protocol not available. A bad option or level was specified in a
       getsockopt or setsockopt system call.
     */
    extern const ENOPROTOOPT:c_int;

    /* No space left on device. A write system call to an ordinary file, the
       creation of a directory or symbolic link, or the creation of a directory
       entry failed because no more disk blocks were available on the file
       system, or the allocation of an inode for a newly created file failed
       because no more inodes were available on the file system.
     */
    extern const ENOSPC:c_int;

    /* Function not implemented. Attempted a system call that is not available
       on this system.
     */
    extern const ENOSYS:c_int;


    /* Socket is not connected. An request to send or receive data was
       disallowed because the socket was not connected and (when
       sending on a datagram socket) no address was
       supplied.
     */
    extern const ENOTCONN:c_int;

    /* Not a directory. A component of the specified pathname existed, but it
       was not a directory, when a directory was expected.
     */
    extern const ENOTDIR:c_int;

    /* Directory not empty. A directory with entries other than '.' and '..'
       was supplied to a remove directory or rename call.
     */
    extern const ENOTEMPTY:c_int;

    extern const ENOTRECOVERABLE:c_int;

    /* Socket operation on non-socket. */
    extern const ENOTSOCK:c_int;

    /* Operation not supported. The attempted operation is not supported for
       the type of object referenced. Usually this occurs when a file
       descriptor refers to a file or socket that cannot support this
       operation, for example, trying to accept a connection on a datagram
       socket.
     */
    extern const ENOTSUP:c_int;

    /* Inappropriate ioctl for device. A control function (e.g. ioctl system
       call) was attempted for a file or special device for which the operation
       was inappropriate.
     */
    extern const ENOTTY:c_int;

    /* Device not configured. Input or output on a special file referred to a
       device that did not exist, or made a request beyond the limits of the
       device. This error may also occur when, for example, a tape drive is
       not online or no disk pack is loaded on a drive.
     */
    extern const ENXIO:c_int;

    /* Operation not supported. The attempted operation is not supported for
       the type of object referenced. Usually this occurs when a file
       descriptor refers to a file or socket that cannot support this
       operation, for example, trying to accept a connection on a datagram
       socket.
     */
    extern const EOPNOTSUPP:c_int;

    /* Value too large to be stored in data type. A numerical result of the
       function was too large to be stored in the caller provided space.
     */
    extern const EOVERFLOW:c_int;

    extern const EOWNERDEAD:c_int;

    /* Operation not permitted. An attempt was made to perform an operation
       limited to processes with appropriate privileges or to the owner of a
       file or other resources.
     */
    extern const EPERM:c_int;

    /* Broken pipe. A write on a pipe, socket or FIFO for which there is no
       process to read the data.
     */
    extern const EPIPE:c_int;

    /* Protocol error. A device or socket encountered an unrecoverable
       protocol error.
     */
    extern const EPROTO:c_int;

    /* Protocol not supported. The protocol has not been configured into the
       system or no implementation for it exists.
     */
    extern const EPROTONOSUPPORT:c_int;

    /* Protocol wrong type for socket. A protocol was specified that does not
       support the semantics of the socket type requested. For example, you
       cannot use the ARPA Internet UDP protocol with type SOCK_STREAM.
     */
    extern const EPROTOTYPE:c_int;

    /* Result too large. A numerical result of the function was too large to
       fit in the available space (perhaps exceeded precision).
     */
    extern const ERANGE:c_int;

    /* Read-only file system. An attempt was made to modify a file or directory
       on a file system that was read-only at the time.
     */
    extern const EROFS:c_int;

    /* Illegal seek. An lseek system call was issued on a socket, pipe or FIFO.
     */
    extern const ESPIPE:c_int;

    /* No such process. No process could be found corresponding to that
       specified by the given process ID.
     */
    extern const ESRCH:c_int;

    /* Stale NFS file handle. An attempt was made to access an open file (on an
       NFS file system) which is now unavailable as referenced by the file
       descriptor. This may indicate the file was deleted on the NFS server or
       some other catastrophic event occurred.
     */
    extern const ESTALE:c_int;

    /* Operation timed out. A connect or send system call failed because the
       connected party did not properly respond after a period of time (The
       timeout period is dependent on the communication protocol).
     */
    extern const ETIMEDOUT:c_int;

    /* Text file busy. The new process was a pure procedure (shared text) file
       which was open for writing by another process, or while the pure
       procedure file was being executed an open system call requested write
       access.
     */
    extern const ETXTBSY:c_int;

    /* Operation would block (may be same value as EAGAIN).
     */
    extern const EWOULDBLOCK:c_int;

    /* Cross-device link. A hard link to a file on another file system was
       attempted.
     */
    extern const EXDEV:c_int;

    /*
       POSIX says that errno is a "modifiable lvalue of type int", but
       for now we only support reading from it, not assigning to it.
    */
    inline proc errno:c_int {
      extern proc chpl_os_posix_errno_val():c_int;
      return chpl_os_posix_errno_val();
    }

    //
    // fcntl.h
    //
    extern const O_ACCMODE:c_int;
    extern const O_APPEND:c_int;
    extern const O_CLOEXEC:c_int;
    extern const O_CREAT:c_int;
    extern const O_DIRECTORY:c_int;
    extern const O_DSYNC:c_int;
    extern const O_EXCL:c_int;
    extern const O_NOCTTY:c_int;
    extern const O_NOFOLLOW:c_int;
    extern const O_NONBLOCK:c_int;
    extern const O_RDONLY:c_int;
    extern const O_RDWR:c_int;
    extern const O_SYNC:c_int;
    extern const O_TRUNC:c_int;
    extern const O_WRONLY:c_int;
    // Note: O_EXEC, O_SEARCH, O_TTY_INIT
    // are documented in POSIX but don't seem to exist on linux
    // Note: O_RSYNC
    // is documented in POSIX but doesn't seem to exist on Mac OS

    extern proc creat(path:c_string, mode:mode_t = 0):c_int;
    inline proc open(path:c_string, oflag:c_int, mode:mode_t = 0:mode_t)
                  :c_int {
      extern proc chpl_os_posix_open(path:c_string, oflag:c_int, mode:mode_t)
                    :c_int;
      return chpl_os_posix_open(path, oflag, mode);
    }

    //
    // stdlib.h
    //
    extern proc getenv(name:c_string):c_ptr(c_char);

    //
    // string.h
    //
    extern proc strerror(errnum:c_int):c_string;
    extern proc strlen(s:c_string):c_size_t;

    //
    // sys/select.h
    //
    extern const FD_SETSIZE:c_int;

    extern record fd_set {};

    proc FD_CLR(fd:c_int, fdset:c_ptr(fd_set)) {
      extern proc chpl_os_posix_FD_CLR(fd:c_int, fdset:c_ptr(fd_set));
      chpl_os_posix_FD_CLR(fd, fdset);
    }

    proc FD_ISSET(fd:c_int, fdset:c_ptr(fd_set)):c_int {
      extern proc chpl_os_posix_FD_ISSET(fd:c_int, fdset:c_ptr(fd_set)):c_int;
      return chpl_os_posix_FD_ISSET(fd, fdset);
    }

    proc FD_SET(fd:c_int, fdset:c_ptr(fd_set)) {
      extern proc chpl_os_posix_FD_SET(fd:c_int, fdset:c_ptr(fd_set));
      chpl_os_posix_FD_SET(fd, fdset);
    }

    proc FD_ZERO(fdset:c_ptr(fd_set)) {
      extern proc chpl_os_posix_FD_ZERO(fdset:c_ptr(fd_set));
      chpl_os_posix_FD_ZERO(fdset);
    }

    // No way around this -- 'select' is a keyword in Chapel.
    extern 'select' proc select_posix(nfds:c_int,
                                      readfds:c_ptr(fd_set),
                                      writefds:c_ptr(fd_set),
                                      errorfds:c_ptr(fd_set),
                                      timeout:c_ptr(struct_timeval)):c_int;

    //
    // sys/stat.h
    //
    inline proc S_IRWXU:mode_t {
      extern proc chpl_os_posix_S_IRWXU():mode_t;
      return chpl_os_posix_S_IRWXU();
    }
    inline proc S_IRUSR:mode_t {
      extern proc chpl_os_posix_S_IRUSR():mode_t;
      return chpl_os_posix_S_IRUSR();
    }
    inline proc S_IWUSR:mode_t {
      extern proc chpl_os_posix_S_IWUSR():mode_t;
      return chpl_os_posix_S_IWUSR();
    }
    inline proc S_IXUSR:mode_t {
      extern proc chpl_os_posix_S_IXUSR():mode_t;
      return chpl_os_posix_S_IXUSR();
    }

    inline proc S_IRWXG:mode_t {
      extern proc chpl_os_posix_S_IRWXG():mode_t;
      return chpl_os_posix_S_IRWXG();
    }
    inline proc S_IRGRP:mode_t {
      extern proc chpl_os_posix_S_IRGRP():mode_t;
      return chpl_os_posix_S_IRGRP();
    }
    inline proc S_IWGRP:mode_t {
      extern proc chpl_os_posix_S_IWGRP():mode_t;
      return chpl_os_posix_S_IWGRP();
    }
    inline proc S_IXGRP:mode_t {
      extern proc chpl_os_posix_S_IXGRP():mode_t;
      return chpl_os_posix_S_IXGRP();
    }

    inline proc S_IRWXO:mode_t {
      extern proc chpl_os_posix_S_IRWXO():mode_t;
      return chpl_os_posix_S_IRWXO();
    }
    inline proc S_IROTH:mode_t {
      extern proc chpl_os_posix_S_IROTH():mode_t;
      return chpl_os_posix_S_IROTH();
    }
    inline proc S_IWOTH:mode_t {
      extern proc chpl_os_posix_S_IWOTH():mode_t;
      return chpl_os_posix_S_IWOTH();
    }
    inline proc S_IXOTH:mode_t {
      extern proc chpl_os_posix_S_IXOTH():mode_t;
      return chpl_os_posix_S_IXOTH();
    }

    inline proc S_ISUID:mode_t {
      extern proc chpl_os_posix_S_ISUID():mode_t;
      return chpl_os_posix_S_ISUID();
    }
    inline proc S_ISGID:mode_t {
      extern proc chpl_os_posix_S_ISGID():mode_t;
      return chpl_os_posix_S_ISGID();
    }
    inline proc S_ISVTX:mode_t {
      extern proc chpl_os_posix_S_ISVTX():mode_t;
      return chpl_os_posix_S_ISVTX();
    }

    extern 'struct chpl_os_posix_struct_stat' record struct_stat {
      var st_dev:dev_t;            // Device.
      var st_ino:ino_t;            // File serial number.
      var st_mode:mode_t;          // File mode.
      var st_nlink:nlink_t;        // Link count.
      var st_uid:uid_t;            // User ID of the file's owner.
      var st_gid:gid_t;            // Group ID of the file's group.
      var st_rdev:dev_t;           // Device number, if device.
      var st_size:off_t;           // Size of file, in bytes.
      var st_atim:struct_timespec; // Last data access timestamp.
      var st_mtim:struct_timespec; // Last data modification timestamp.
      var st_ctim:struct_timespec; // Last file status change timestamp.
      var st_blksize:blksize_t;    // Optimal block size for I/O.
      var st_blocks:blkcnt_t;      // Number 512-byte blocks allocated.
    }

    extern proc chmod(path:c_string, mode:mode_t):c_int;
    extern 'chpl_os_posix_stat' proc stat(path:c_string,
                                          buf:c_ptr(struct_stat)):c_int;

    //
    // sys/time.h
    //
    extern "struct timeval" record struct_timeval {
      var tv_sec:time_t;       // seconds since Jan. 1, 1970
      var tv_usec:suseconds_t; // and microseconds
    }

    extern "struct timezone" record struct_timezone {
      var tz_minuteswest:c_int; // of Greenwich
      var tz_dsttime:c_int;     // type of dst correction to apply
    };

    extern proc gettimeofday(tp:c_ptr(struct_timeval),
                             tzp:c_ptr(struct_timezone)):c_int;

    //
    // time.h
    //
    extern "struct tm" record struct_tm {
      var tm_sec:c_int;   // Seconds [0,60] (60 allows for leap seconds)
      var tm_min:c_int;   // Minutes [0,59]
      var tm_hour:c_int;  // Hour [0,23]
      var tm_mday:c_int;  // Day of month [1,31]
      var tm_mon:c_int;   // Month of year [0,11]
      var tm_year:c_int;  // Years since 1900
      var tm_wday:c_int;  // Day of week [0,6] (Sunday =0)
      var tm_yday:c_int;  // Day of year [0,365]
      var tm_isdst:c_int; // Daylight Savings flag
    };

    extern proc asctime(timeptr:c_ptr(struct_tm)):c_ptr(c_char);
    extern proc asctime_r(timeptr:c_ptr(struct_tm), buf:c_ptr(c_char))
                  :c_ptr(c_char);
    extern proc localtime(timer:c_ptr(time_t)):c_ptr(struct_tm);
    extern proc localtime_r(timer:c_ptr(time_t), result:c_ptr(struct_tm))
                  :c_ptr(struct_tm);
    extern proc time(tloc:c_ptr(time_t)):time_t;

    //
    // unistd.h
    //
    extern proc close(fildes:c_int):c_int;
    extern proc pipe(fildes:c_ptr(c_array(c_int, 2))):c_int;
    extern proc read(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;
    extern proc write(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;

  } // end POSIX
}
