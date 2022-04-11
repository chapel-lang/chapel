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
   Support for operating system interfaces.

   This module provides Chapel declarations for the constants, types,
   and functions defined by various operating systems' programmatic
   interfaces.  It is not complete for any operating system, but does
   define those things that have been needed so far in implementing
   other Chapel modules and user programs.  It is expected to grow as
   needed.  In all respects (naming, capitalization, types, semantics)
   the symbols defined here should match their corresponding operating
   system definitions.  For documentation on these symbols, please see
   the operating system manual pages.
 */
module OS {
  /*
     Definitions corresponding to POSIX, specifically POSIX.1-2017.
     Standard: https://pubs.opengroup.org/onlinepubs/9699919799/
  */
  pragma "no doc"
  module POSIX {
    public use CTypes;

    //
    // sys/types.h
    //
    extern type mode_t;

    // Users definitely need |(mode_t, mode_t).  We may want others in
    // the future.
    inline operator :(x:mode_t, type t:int) return __primitive("cast", t, x);
    inline operator :(x:int, type t:mode_t) return __primitive("cast", t, x);
    inline operator |(a: mode_t, b: mode_t) return (a:int | b:int):mode_t;

    extern type time_t;
    inline operator :(x:time_t, type t:int) return __primitive("cast", t, x);
    inline operator :(x:int, type t:time_t) return __primitive("cast", t, x);

    extern type suseconds_t;
    inline operator :(x:suseconds_t, type t:int)
      return __primitive("cast", t, x);
    inline operator :(x:int, type t:suseconds_t)
      return __primitive("cast", t, x);

    //
    // errno.h
    //
    extern const E2BIG:c_int;
    extern const EACCES:c_int;
    extern const EADDRINUSE:c_int;
    extern const EADDRNOTAVAIL:c_int;
    extern const EAFNOSUPPORT:c_int;
    extern const EAGAIN:c_int;
    extern const EALREADY:c_int;
    extern const EBADF:c_int;
    extern const EBADMSG:c_int;
    extern const EBUSY:c_int;
    extern const ECANCELED:c_int;
    extern const ECHILD:c_int;
    extern const ECONNABORTED:c_int;
    extern const ECONNREFUSED:c_int;
    extern const ECONNRESET:c_int;
    extern const EDEADLK:c_int;
    extern const EDESTADDRREQ:c_int;
    extern const EDOM:c_int;
    extern const EDQUOT:c_int;
    extern const EEXIST:c_int;
    extern const EFAULT:c_int;
    extern const EFBIG:c_int;
    extern const EHOSTUNREACH:c_int;
    extern const EIDRM:c_int;
    extern const EILSEQ:c_int;
    extern const EINPROGRESS:c_int;
    extern const EINTR:c_int;
    extern const EINVAL:c_int;
    extern const EIO:c_int;
    extern const EISCONN:c_int;
    extern const EISDIR:c_int;
    extern const ELOOP:c_int;
    extern const EMFILE:c_int;
    extern const EMLINK:c_int;
    extern const EMSGSIZE:c_int;
    extern const EMULTIHOP:c_int;
    extern const ENAMETOOLONG:c_int;
    extern const ENETDOWN:c_int;
    extern const ENETRESET:c_int;
    extern const ENETUNREACH:c_int;
    extern const ENFILE:c_int;
    extern const ENOBUFS:c_int;
    extern const ENODEV:c_int;
    extern const ENOENT:c_int;
    extern const ENOEXEC:c_int;
    extern const ENOLCK:c_int;
    extern const ENOLINK:c_int;
    extern const ENOMEM:c_int;
    extern const ENOMSG:c_int;
    extern const ENOPROTOOPT:c_int;
    extern const ENOSPC:c_int;
    extern const ENOSYS:c_int;
    extern const ENOTCONN:c_int;
    extern const ENOTDIR:c_int;
    extern const ENOTEMPTY:c_int;
    extern const ENOTRECOVERABLE:c_int;
    extern const ENOTSOCK:c_int;
    extern const ENOTSUP:c_int;
    extern const ENOTTY:c_int;
    extern const ENXIO:c_int;
    extern const EOPNOTSUPP:c_int;
    extern const EOVERFLOW:c_int;
    extern const EOWNERDEAD:c_int;
    extern const EPERM:c_int;
    extern const EPIPE:c_int;
    extern const EPROTO:c_int;
    extern const EPROTONOSUPPORT:c_int;
    extern const EPROTOTYPE:c_int;
    extern const ERANGE:c_int;
    extern const EROFS:c_int;
    extern const ESPIPE:c_int;
    extern const ESRCH:c_int;
    extern const ESTALE:c_int;
    extern const ETIMEDOUT:c_int;
    extern const ETXTBSY:c_int;
    extern const EWOULDBLOCK:c_int;
    extern const EXDEV:c_int;

    // POSIX says that errno is a "modifiable lvalue of type int", but
    // for now we only support reading from it, not assigning to it.
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
    extern const O_EXEC:c_int;
    extern const O_NOCTTY:c_int;
    extern const O_NOFOLLOW:c_int;
    extern const O_NONBLOCK:c_int;
    extern const O_RDONLY:c_int;
    extern const O_RDWR:c_int;
    extern const O_RSYNC:c_int;
    extern const O_SEARCH:c_int;
    extern const O_SYNC:c_int;
    extern const O_TRUNC:c_int;
    extern const O_TTY_INIT:c_int;
    extern const O_WRONLY:c_int;

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
    extern proc strlen(s:c_string):c_size_t;

    //
    // sys/stat.h
    //
    extern const S_IRWXU: mode_t;
    extern const S_IRUSR: mode_t;
    extern const S_IWUSR: mode_t;
    extern const S_IXUSR: mode_t;

    extern const S_IRWXG: mode_t;
    extern const S_IRGRP: mode_t;
    extern const S_IWGRP: mode_t;
    extern const S_IXGRP: mode_t;

    extern const S_IRWXO: mode_t;
    extern const S_IROTH: mode_t;
    extern const S_IWOTH: mode_t;
    extern const S_IXOTH: mode_t;

    extern const S_ISUID: mode_t;
    extern const S_ISGID: mode_t;
    extern const S_ISVTX: mode_t;

    extern proc chmod(path:c_string, mode:mode_t):c_int;

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
    extern proc read(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;
    extern proc write(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;

  } // end POSIX
}
