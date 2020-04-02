/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _CYGWIN_CHPLSYS_H_
#define _CYGWIN_CHPLSYS_H_

#include "../chplsys.h"

#define chplGetPageSize() 4096

// Why do we hardcode the pagesize into the cygwin implementation?
// 
// From the Cygwin mailing list archives:
//
// On Jun 15 15:09, Dave Korn wrote:
// > On 15 June 2006 14:56, Ehren Jarosek wrote:
// > 
// > > I don't know if this is something I am doing wrong or an issue.
// > > 
// > > When compiling under cygwin sysconf(_SC_PAGESIZE) returns 65536 (64k)
// > > memory page size.  My understanding is that:
// > > 
// > > sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES)
// > > 
// > > should yield the total physical memory size of the machine.  However,
// > > when I do this it yields a very large number (actually overflows my
// > > long).  However, if I multiply sysconf(_SC_PHYS_PAGES) * 4096 it yields
// > > the correct size.
// > 
// > Alas there is a problem with the definition of sysconf: it is
// > supposed to be the size of the unit of granularity of mmap'ing, but
// > it is also supposed to be the size of a single pageframe of memory.
// > [...]
//
// _SC_PAGESIZE is only for indicating the page size as used in calls to
// mmap(2).  POSIX does not demand that _SC_PAGESIZE is actually the
// physical page size.
//
// Two quotes from the Linux man pages:
//
//   $ man getpagesize
//   [...]
//    The  function  getpagesize()  returns  the  number of bytes in a page,
//    where a "page" is the thing used where it says in the  description  of
//    mmap(2) that files are mapped in page-sized units.
//
//    The size of the kind of pages that mmap uses, is found using
//
//        #include <unistd.h>
//        long sz = sysconf(_SC_PAGESIZE);
//
//
//   $ man sysconf
//   [...]
//    These values also exist, but may not be standard.
//
//    - _SC_PHYS_PAGES
//       The number of pages of physical memory.  Note that it is possi-
//       ble   for   the   product  of  this  value  and  the  value  of
//       _SC_PAGE_SIZE to overflow.
//
//
// So, actually Ehren's application works on Linux just coincidentally,
// since it make invalid assumptions.
//
//
// Corinna
//
// -- 
// Corinna Vinschen                  Please, send mails regarding Cygwin to
// Cygwin Project Co-Leader          cygwin AT cygwin DOT com
// Red Hat

#endif
