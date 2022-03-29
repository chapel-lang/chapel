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

    // stdlib.h: functions
    extern proc getenv(name:c_string):c_string;

  } // end POSIX
}
