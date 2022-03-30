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
    use CTypes;

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
    extern proc getenv(name:c_string):c_string;

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
    extern "struct timeval" record timeval {
      var tv_sec:time_t;       // seconds since Jan. 1, 1970
      var tv_usec:suseconds_t; // and microseconds
    }

    extern "struct timezone" record timezone {
      var tz_minuteswest:c_int; // of Greenwich
      var tz_dsttime:c_int;     // type of dst correction to apply
    };

    extern proc gettimeofday(ref tp: timeval, tzp:c_void_ptr):c_int;

    //
    // unistd.h
    //
    extern proc close(fildes:c_int):c_int;
    extern proc read(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;
    extern proc write(fildes:c_int, buf:c_void_ptr, size:c_size_t):c_ssize_t;

  } // end POSIX
}
