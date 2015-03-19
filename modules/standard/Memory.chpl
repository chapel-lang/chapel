/*
 * Copyright 2004-2015 Cray Inc.
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
  regarding memory usage.  Except for :proc:`locale.physicalMemory` as
  noted below, all of the procedures in :mod:`Memory` require memory
  tracking to be enabled.  This can be done by setting at least one of
  the following configuration variables at execution time, using either
  the ``--configVarName=value`` or ``-sconfigVarName=value`` command
  line options when a program is run.

  ``memTrack``: :type:`bool`:
    Enable memory tracking.  This causes memory allocations and
    deallocations to be internally tracked, in order to support
    logging them on the fly and/or producing any of several kinds of
    memory-related reports, described below.

  ``memLeaksTable``: :type:`bool`:
    Enable memory tracking and produce detailed information about
    leaked memory by invoking :proc:`printMemTable` implicitly when
    when the program terminates normally.

  ``memLeaks``: :type:`bool`:
    Enable memory tracking and produce summary information about
    leaked memory by invoking :proc:`printMemLeaksTable` implicitly
    when the program terminates normally.

  ``memLeaksLog``: :type:`c_string`:
    Enable memory tracking and produce a report to the named file when
    the program terminates normally.
    The report contains the compilation command used to build the
    program, the execution command used to run it, the summary
    allocation statistics produced by :proc:`printMemStat`, and the
    summary information about leaked memory produced by
    :proc:`printMemLeaksTable`.

  ``memStats``: :type:`bool`:
    Enable memory tracking and produce summary memory statistics by
    invoking :proc:`printMemStat` implicitly at normal program
    termination.

  ``memMax``: :type:`uint`:
    If the value is greater than 0 (zero), enable memory tracking
    and use the value as a limit on the number of bytes of memory
    that can be allocated per top-level (network-connected) locale.
    If during execution the amount of allocated memory exceeds this
    limit on any locale, halt the program with a message saying so.

  The following two config variables do not enable memory tracking;
  they only modify how it is done.


  ``memThreshold``: :type:`uint`:
    If this is set to a value greater than 0 (zero), only allocation
    requests larger than this are tracked and/or reported.

  ``memLog``: :type:`c_string`:
    Memory reporting is written to this file.  By default it is the
    ``stdout`` associated with the process (not the Chapel channel
    with the same name).  Setting this config variable to a file path
    causes the reporting to be written to that named file instead.
 */
module Memory {

pragma "no doc"
pragma "insert line file info" extern proc chpl_memoryUsed(): uint(64);

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

  *Note:* This is the only procedure in the :mod:`Memory` module which
  does not need memory tracking enabled in order to work properly.

  :arg unit: Units in which the returned value is to be expressed.
  :type unit: MemUnits
  :arg retType: Type of the returned value.  Defaults to `int(64)`.
  :type retType: type
  :returns: Size of physical memory on the locale where the call is made.
  :rtype: :type:`retType`
 */
proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  extern proc chpl_bytesPerLocale(): uint(64);

  var bytesInLocale: uint(64);

  on this do bytesInLocale = chpl_bytesPerLocale();

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
  :type thresh: int
*/
proc printMemTable(thresh=0) {
  pragma "insert line file info" 
  extern proc chpl_printMemTable(thresh);

  chpl_printMemTable(thresh);
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
proc printMemLeaksTable() {
  extern proc chpl_printLeakedMemTable();
  chpl_printLeakedMemTable();
}

/*
  Print summary memory statistics to ``memLog``.  The report contains
  a section for each top-level locale showing the number of bytes of
  memory currently allocated, the maximum number allocated at any
  point during execution (the high-water mark), and the sum of the
  sizes of all allocation and deallocation requests.
*/
proc printMemStat() {
  pragma "insert line file info"
  extern proc chpl_printMemStat();

  chpl_printMemStat();
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
