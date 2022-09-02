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

import OS.errorCode;

/* A type storing an error code or an error message.
   A syserr can be compared using == or != to an qio_err_t (ie integer error code)
   or to another syserr. A syserr can be cast to or from an qio_err_t. It can be
   assigned the value of an qio_err_t or another syserr. In addition, syserr can be
   checked directly in an if statement like so:

   .. code-block:: chapel

     var err: syserr;
     if err then do writeln("err contains an error, ie err != ENOERR");
     if !err then do writeln("err does not contain an error; err == ENOERR");

   When a :type:`syserr` formal has default intent, the actual is copied to the
   formal upon a function call and the formal cannot be assigned within the
   function.

   The default value of the :type:`syserr` type is undefined.

 */
deprecated "'SysBasic.syserr' has been deprecated; please use 'OS.errorCode' instead."
extern type syserr; // = c_int, opaque so we can manually override ==,!=,etc

/* An integral error code. This is really just a `c_int`, but code is
   clearer if you use qio_err_t to indicate arguments, variables, and return types
   that are system error codes.
 */
deprecated "'qio_err_t' has been deprecated; please use a 'CTypes.c_int' instead."
extern type qio_err_t = c_int;

/* A system file descriptor. This is really just a `c_int`, but code is
   clearer if you use fd_t to indicate arguments, variables, and return types
   that are system file descriptors.
 */
extern type fd_t = c_int;

/* The error code indicating that no error occurred (Chapel specific) */
inline proc ENOERR return 0:c_int;

// end of file
private extern proc chpl_macro_int_EEOF():c_int;

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

}
