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

  } // end POSIX
}
