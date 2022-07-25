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

/* Types for low-level system error integration.

   This module defines the error types :type:`syserr` and :type:`qio_err_t`.

   When should one use :type:`syserr` and when should one use :type:`qio_err_t`?
   :type:`qio_err_t` is a system error code (a `c_int` by a nicer name to
   indicate its purpose). :type:`syserr` is an enhanced error that might also
   include an error message. All user-facing Chapel library code, or user
   Chapel code, should generally use :type:`syserr`. When wrapping functions
   or declaring them in C, use :type:`qio_err_t` to indicate that a function is
   returning an error code.

   A note about the error code documentation in this file. Error descriptions
   for system errors are included here for convenience only. Other
   documentation, such as system man pages or the POSIX specification are the
   authority on system error codes.
 */

module SysBasic {

/* BASIC TYPES */
use CTypes;

/* A type storing an error code or an error message.
   A syserr can be compared using == or != to an qio_err_t (ie integer error code)
   or to another syserr. A syserr can be cast to or from an qio_err_t. It can be
   assigned the value of an qio_err_t or another syserr. In addition, syserr can be
   checked directly in an if statement like so:

   .. code-block:: chapel

     var err: syserr;
     if err then do writeln("err contains an error, ie err != ENOERR");
     if !err then do writeln("err does not contain an error; err == ENOERR");

xs
   When a :type:`syserr` formal has default intent, the actual is copied to the
   formal upon a function call and the formal cannot be assigned within the
   function.

   The default value of the :type:`syserr` type is undefined.

 */
extern type syserr; // = c_int, opaque so we can manually override ==,!=,etc

/* An integral error code. This is really just a `c_int`, but code is
   clearer if you use qio_err_t to indicate arguments, variables, and return types
   that are system error codes.
 */
// deprecated "'qio_err_t' has been deprecated; please use a 'CTypes.c_int' instead."
// extern type qio_err_t = c_int;
inline proc qio_err_t type {
  compilerWarning("'qio_err_t' is deprecated; please use a 'CTypes.c_int' instead");
  return c_int;
}

/* A system file descriptor. This is really just a `c_int`, but code is
   clearer if you use fd_t to indicate arguments, variables, and return types
   that are system file descriptors.
 */
extern type fd_t = c_int;


// error numbers

private extern proc qio_err_eq(a:syserr, b:syserr):c_int;
private extern proc qio_err_to_int(a:syserr):int(32);
private extern proc qio_int_to_err(a:int(32)):syserr;
private extern proc qio_err_iserr(a:syserr):c_int;

/* The error code indicating that no error occurred (Chapel specific) */
inline proc ENOERR return 0:c_int;

// When qio_err_t is no longer just int(32), will need to add cases for qio_err_t too.
pragma "no doc"
inline operator syserr.==(a: syserr, b: syserr) {
  return (qio_err_eq(a,b) != 0:c_int);
}
pragma "no doc"
inline operator syserr.==(a: syserr, b: int(32))
  return (qio_err_to_int(a) == b:int(32));
pragma "no doc"
inline operator syserr.==(a: syserr, b: int(64))
  return (qio_err_to_int(a) == b:int(32));
pragma "no doc"
inline operator syserr.==(a: int(32), b: syserr)
  return (a:int(32) == qio_err_to_int(b));
pragma "no doc"
inline operator syserr.==(a: int(64), b: syserr)
  return (a:int(32) == qio_err_to_int(b));
pragma "no doc"
inline operator syserr.!=(a: syserr, b: syserr) return !(a == b);
pragma "no doc"
inline operator syserr.!=(a: syserr, b: int(32)) return !(a == b);
pragma "no doc"
inline operator syserr.!=(a: syserr, b: int(64)) return !(a == b);
pragma "no doc"
inline operator syserr.!=(a: int(32), b: syserr) return !(a == b);
pragma "no doc"
inline operator syserr.!=(a: int(64), b: syserr) return !(a == b);
pragma "no doc"
inline operator syserr.!(a: syserr) return (qio_err_iserr(a) == 0:c_int);
pragma "no doc"
inline proc syserr.chpl_cond_test_method() return (qio_err_iserr(this) != 0:c_int);
pragma "no doc"
inline operator :(x: syserr, type t: int(32)) return qio_err_to_int(x);
pragma "no doc"
inline operator :(x: syserr, type t: int(64)) return qio_err_to_int(x):int(64);
pragma "no doc"
inline operator :(x: int(32), type t: syserr) return qio_int_to_err(x);
pragma "no doc"
inline operator :(x: int(64), type t: syserr) return qio_int_to_err(x:int(32));
pragma "no doc"
inline operator syserr.=(ref ret:syserr, x:syserr) { __primitive("=", ret, x); }
pragma "no doc"
inline operator syserr.=(ref ret:syserr, x:int(32))
{ __primitive("=", ret, qio_int_to_err(x)); }
pragma "no doc"
inline operator syserr.=(ref ret:syserr, x:int(64))
{ __primitive("=", ret, qio_int_to_err(x:int(32))); }
pragma "no doc"
inline operator syserr.=(ref ret:qio_err_t, x:syserr)
{ __primitive("=", ret, qio_err_to_int(x):qio_err_t); }

// end of file
private extern proc chpl_macro_int_EEOF():qio_err_t;

/* An error code indicating the end of file has been reached (Chapel specific)
 */
inline proc EEOF return chpl_macro_int_EEOF():c_int;

private extern proc chpl_macro_int_ESHORT():c_int;

/* An error code indicating that the end of file or the end of the
   input was reached before the requested amount of data could be read.
   (Chapel specific)
  */
inline proc ESHORT return chpl_macro_int_ESHORT():c_int;

private extern proc chpl_macro_int_EFORMAT():c_int;

/* An error code indicating a format error; for example when reading a quoted
   string literal, this would be returned if we never encountered the
   opening quote. (Chapel specific)
  */
inline proc EFORMAT return chpl_macro_int_EFORMAT():c_int;

// system error numbers

// The descriptions of these were created by combining the FreeBSD
// manual and the linux manuals here:
// http://www.freebsd.org/cgi/man.cgi?query=errno&apropos=0&sektion=0&manpath=FreeBSD+10.1-RELEASE&arch=default&format=html
// http://linux.die.net/man/3/errno
// Verified that the POSIX annotations are correct with POSIX.1-2008:
// http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
// Note that these error descriptions are reproduced here for convenience
// only. These descriptions aren't the authority on the matter.

/* Invalid exchange (linux only) */
extern const EBADE:c_int;

/* File descriptor in bad state (linux only) */
extern const EBADFD:c_int;

/* Invalid request descriptor (linux only) */
extern const EBADR:c_int;

/* Invalid request code (linux only) */
extern const EBADRQC:c_int;

/* Invalid slot (linux only) */
extern const EBADSLT:c_int;

/* Channel number out of range (linux only) */
extern const ECHRNG:c_int;

/* Communication error on send (linux only) */
extern const ECOMM:c_int;

/* Host is down. A socket operation failed because the destination host was
   down. (linux, FreeBSD)
 */
extern const EHOSTDOWN:c_int;

/* Is a named type file (linux only) */
extern const EISNAM:c_int;

/* Key has expired (linux only) */
extern const EKEYEXPIRED:c_int;

/* Key was rejected by service (linux only) */
extern const EKEYREJECTED:c_int;

/* Key has been revoked (linux only) */
extern const EKEYREVOKED:c_int;

/* Level 2 halted (linux only) */
extern const EL2HLT:c_int;

/* Level 2 not synchronized (linux only) */
extern const EL2NSYNC:c_int;

/* Level 3 halted (linux only) */
extern const EL3HLT:c_int;

/* Level 3 halted (linux only) */
extern const EL3RST:c_int;

/* Cannot access a needed shared library (linux only) */
extern const ELIBACC:c_int;

/* Accessing a corrupted shared library (linux only) */
extern const ELIBBAD:c_int;

/* Attempting to link in too many shared libraries (linux only) */
extern const ELIBMAX:c_int;

/* lib section in a.out corrupted (linux only) */
extern const ELIBSCN:c_int;

/* Cannot exec a shared library directly (linux only) */
extern const ELIBEXEC:c_int;

/* Wrong medium type (linux only) */
extern const EMEDIUMTYPE:c_int;

/* No  message is available on the STREAM head read queue (POSIX.1)
 */
extern const ENODATA:c_int;

/* Required key not available (linux only) */
extern const ENOKEY:c_int;

/* No medium found (linux only) */
extern const ENOMEDIUM:c_int;

/* Machine is not on the network (linux only)
 */
extern const ENONET:c_int;

/* Package not installed  (linux only)
 */
extern const ENOPKG:c_int;

/* No STREAM resources (POSIX.1 XSI STREAMS option) */
extern const ENOSR:c_int;

/* Not a STREAM (POSIX.1 XSI STREAMS option) */
extern const ENOSTR:c_int;

/* Block device required.
   A block device operation was attempted on a non-block device or file.
   (linux, FreeBSD)
 */
extern const ENOTBLK:c_int;

/* Name not unique on network (linux only) */
extern const ENOTUNIQ:c_int;

/* Protocol family not supported. The protocol family has not been configured
   into the system or no implementation for it exists. (linux, FreeBSD)
 */
extern const EPFNOSUPPORT:c_int;

/* Remote address changed (linux only) */
extern const EREMCHG:c_int;

/* Object is remote (linux only) */
extern const EREMOTE:c_int;

/* Remote I/O error (linux only) */
extern const EREMOTEIO:c_int;

/* Interrupted system call should be restarted (linux only) */
extern const ERESTART:c_int;

/* Can't send after socket shutdown. A request to send data was disallowed
   because the socket had already been shut down with a previous shutdown system
   call.
 */
extern const ESHUTDOWN:c_int;

/* Socket type not supported. The support for the socket type has not been
   configured into the system or no implementation for it exists.
   (linux, FreeBSD)
 */
extern const ESOCKTNOSUPPORT:c_int;

/* Streams pipe error (linux only) */
extern const ESTRPIPE:c_int;

/* Timer expired (POSIX.1 XSI STREAMS option)
 */
extern const ETIME:c_int;

/* Structure needs cleaning (linux only) */
extern const EUCLEAN:c_int;

/* Protocol driver not attached (linux only) */
extern const EUNATCH:c_int;

/* Too many users. The quota system ran out of table entries.
   (linux, FreeBSD)
 */
extern const EUSERS:c_int;

/* Exchange full (linux only) */
extern const EXFULL:c_int;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const E2BIG:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EACCES:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EADDRINUSE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EADDRNOTAVAIL:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EAFNOSUPPORT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EAGAIN:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EALREADY:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EBADF:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EBADMSG:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EBUSY:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ECANCELED:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ECHILD:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ECONNABORTED:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ECONNREFUSED:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ECONNRESET:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EDEADLK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EDESTADDRREQ:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EDOM:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EDQUOT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EEXIST:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EFAULT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EFBIG:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EHOSTUNREACH:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EIDRM:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EILSEQ:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EINPROGRESS:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EINTR:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EINVAL:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EIO:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EISCONN:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EISDIR:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ELOOP:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EMFILE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EMLINK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EMSGSIZE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EMULTIHOP:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENAMETOOLONG:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENETDOWN:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENETRESET:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENETUNREACH:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENFILE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOBUFS:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENODEV:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOENT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOEXEC:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOLCK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOLINK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOMEM:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOMSG:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOPROTOOPT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOSPC:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOSYS:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTCONN:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTDIR:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTEMPTY:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTSOCK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTSUP:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENOTTY:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ENXIO:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EOPNOTSUPP:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EOVERFLOW:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EPERM:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EPIPE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EPROTO:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EPROTONOSUPPORT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EPROTOTYPE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ERANGE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EROFS:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ESPIPE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ESRCH:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ESTALE:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ETIMEDOUT:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const ETXTBSY:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EWOULDBLOCK:qio_err_t;

deprecated "POSIX error codes defined in 'SysBasic' are deprecated; please use them from 'OS.POSIX' instead"
extern const EXDEV:qio_err_t;
}
