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

/*
  The :mod:`Memory` module provides procedures which report information
  about memory usage.  With one exception, to use these procedures you
  must enable memory tracking.  Do this by setting one or more of the
  config vars below, using appropriate ``--configVarName=value`` or
  ``-sconfigVarName=value`` command line options when you run the
  program.  If memory tracking is not enabled, calling any procedure
  described here, other than :proc:`locale.physicalMemory`, will cause
  the program to halt with an error message.

  ``memTrack``: `bool`:
    Enable memory tracking.  This causes memory allocations and
    deallocations to be internally tracked, to support logging them on
    the fly and/or producing any of several kinds of memory-related
    reports, described below.

  ``memLeaks``: `bool`:
    Enable memory tracking and produce detailed information about
    leaked memory by invoking :proc:`printMemAllocs` implicitly when
    the program terminates normally.

  ``memLeaksByType``: `bool`:
    Enable memory tracking and produce summary information about
    leaked memory by invoking :proc:`printMemAllocsByType` implicitly
    when the program terminates normally.

  ``memLeaksLog``: `string`:
    Enable memory tracking and append a report to the named file when
    the program terminates normally.
    The report contains the compilation command used to build the
    program, the execution command used to run it, the summary
    allocation statistics produced by :proc:`printMemAllocStats`, and the
    summary information about leaked memory produced by
    :proc:`printMemAllocsByType`.
    

  ``memStats``: `bool`:
    Enable memory tracking and produce summary memory statistics by
    invoking :proc:`printMemAllocStats` implicitly at normal program
    termination.  Note that for a multi-locale run, each top-level
    locale reports its own memory statistics and these reports may
    appear in any order in the program output (and possibly even
    interleaved, though we have gone to some effort to avoid that).
    These statistics can be put in order by sorting the output lines
    that begin with the string ``memStats:``.

  ``memMax``: `uint`:
    If the value is greater than 0 (zero), enable memory tracking
    and use the value as a limit on the number of bytes of memory
    that can be allocated per top-level (network-connected) locale.
    If during execution the amount of allocated memory exceeds this
    limit on any locale, halt the program with a message saying so.

  The following two config variables do not enable memory tracking;
  they only modify how it is done.


  ``memThreshold``: `uint`:
    If this is set to a value greater than 0 (zero), only allocation
    requests larger than this are tracked and/or reported.

  ``memLog``: `string`:
    Memory reporting is written to this file.  By default it is the
    ``stdout`` associated with the process (not the Chapel channel
    with the same name).  Setting this config variable to a file path
    causes the reporting to be written to that named file instead.
    In multilocale executions each top-level locale produces output
    to its own file, with a dot ('.') and the locale ID appended to
    this path.
 */
module Memory {

pragma "insert line file info" private extern proc chpl_memoryUsed(): uint(64);

/*
  The amount of memory returned by :proc:`locale.physicalMemory` can
  be expressed either as individual bytes or as chunks of 2**10,
  2**20, or 2**30 bytes.
 */
enum MemUnits {Bytes, KB, MB, GB};

/*
  How much physical memory is present on this locale?

  Note that this is a physical quantity and does not take into account
  things like virtual memory support which might allow for allocating
  a larger amount, or system or user limits which might prevent
  allocating so much.

  *Note:* Unlike the other procedures in the :mod:`Memory` module, this
  one does not require memory tracking to be enabled.

  :arg unit: Units in which the returned value is to be expressed.
  :type unit: :type:`~Memory.MemUnits`
  :arg retType: Type of the returned value.  Defaults to `int(64)`.
  :type retType: `type`
  :returns: Size of physical memory on the locale where the call is made.
  :rtype: `retType`
 */
proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  extern proc chpl_sys_physicalMemoryBytes(): uint(64);

  var bytesInLocale: uint(64);

  on this do bytesInLocale = chpl_sys_physicalMemoryBytes();

  var retVal: retType;
  select (unit) {
    when MemUnits.Bytes do retVal = bytesInLocale:retType;
    when MemUnits.KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MemUnits.MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when MemUnits.GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }

  return retVal;
}

/*
  How much memory is this program currently using on this locale?

  This is the amount of memory known to be currently allocated on the
  calling top-level (network-connected) locale by the program, through
  Chapel mechanisms.  It does not include memory allocated directly
  from the system, outside of Chapel mechanisms, such as allocations
  made by code written in other languages and linked into the program.

  :returns: Amount of allocated memory, in bytes.
  :rtype: `uint(64)`
 */
proc memoryUsed() {
  return chpl_memoryUsed();
}

/*
  Print detailed information about allocated memory to ``memLog``.
  The report contains a section for each top-level locale, containing
  a table of entries for the allocations made on that locale.  Each
  entry shows the source file and line at which the allocation was
  requested, the address and size (bytes) of the allocated space, and
  a description of the type of information the requesting code said it
  was going to store there.

  :arg thresh: Do not print entries whose size is less than this.
    Defaults to 0.
  :type thresh: `int`
*/
proc printMemAllocs(thresh=0) {
  pragma "insert line file info" 
  extern proc chpl_printMemAllocs(thresh);

  chpl_printMemAllocs(thresh);
}

/*
  Print summary information about allocated memory to ``memLog``.  The
  report contains a section for each top-level locale, containing a
  table of entries, one for each different allocation type for which
  at least one allocation exists on that locale.  The entries show the
  type (that is, the string the code requesting the allocation used to
  describe what it would store there) and the total number of
  allocations and bytes allocated for that type.
*/
proc printMemAllocsByType() {
  pragma "insert line file info" 
  extern proc chpl_printMemAllocsByType();

  chpl_printMemAllocsByType();
}

/*
  Print summary memory statistics to ``memLog``.  The report contains
  a section for each top-level locale showing the number of bytes of
  memory currently allocated, the maximum number allocated at any
  point during execution (the high-water mark), and the sum of the
  sizes of all allocation and deallocation requests.
*/
proc printMemAllocStats() {
  pragma "insert line file info"
  extern proc chpl_printMemAllocStats();

  chpl_printMemAllocStats();
}

/*
  Start on-the-fly reporting of memory allocations and deallocations
  done on any locale.  Continue reporting until :proc:`stopVerboseMem`
  is called.

  The reporting output consists of a single line describing each
  memory allocation or deallocation, written to ``memLog``.
 */
proc startVerboseMem() { 
  extern proc chpl_startVerboseMem();
  chpl_startVerboseMem();
}

/*
  Stop on-the-fly reporting of memory allocations and deallocations
  done on any locale.
 */
proc stopVerboseMem() {
  extern proc chpl_stopVerboseMem();
  chpl_stopVerboseMem();
}

/*
  Start on-the-fly reporting of memory allocations and deallocations
  done on this locale.  Continue reporting until
  :proc:`stopVerboseMemHere` is called.

  The reporting output consists of a single line describing each
  memory allocation or deallocation, written to ``memLog``.
 */
proc startVerboseMemHere() {
  extern proc chpl_startVerboseMemHere();
  chpl_startVerboseMemHere();
}

/*
  Stop on-the-fly reporting of memory allocations and deallocations
  done on this locale.
 */
proc stopVerboseMemHere() {
  extern proc chpl_stopVerboseMemHere();
  chpl_stopVerboseMemHere();
}

}
