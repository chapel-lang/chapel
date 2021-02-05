/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
  The :mod:`Memory` module provides submodules that contain operations
  related to memory usage and memory initialization.

  .. warning::

    In previous releases, the :mod:`Memory` module contained procedures
    which reported information about memory usage. These procedures are now
    deprecated - please use the equivalents in the :mod:`Diagnostics`
    submodule instead.

 */
module Memory {

include module Diagnostics;

pragma "insert line file info"
private extern proc chpl_memoryUsed(): uint(64);

/*
  .. warning::

    This enum is deprecated - please use
    :enum:`Diagnostics.MemUnits`.

  The amount of memory returned by :proc:`locale.physicalMemory` can
  be expressed either as individual bytes or as chunks of 2**10,
  2**20, or 2**30 bytes.
 */
enum MemUnits {Bytes, KB, MB, GB};

/*
  .. warning::

    This method is deprecated - please use
    :proc:`Diagnostics.locale.physicalMemory()`.

  How much physical memory is present on this locale?

  This quantity does not take into account things like virtual memory support
  which might allow for allocating a larger amount, or system or user limits
  which might prevent allocating so much.

  .. note::

    Unlike the other procedures in the :mod:`Memory` module, this
    one does not require memory tracking to be enabled.

  :arg unit: Units in which the returned value is to be expressed.
  :type unit: :type:`~Memory.MemUnits`
  :arg retType: Type of the returned value.  Defaults to `int(64)`.
  :type retType: `type`
  :returns: Size of physical memory on the locale where the call is made.
  :rtype: `retType`
 */
proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  compilerWarning('Memory.locale.physicalMemory() is deprecated - ',
                  'please use Memory.Diagnostics.locale.physicalMemory()');

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
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.memoryUsed()`.

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
  compilerWarning('Memory.memoryUsed() is deprecated - please use ',
                  'Memory.Diagnostics.memoryUsed()');

  return chpl_memoryUsed();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.printMemAllocs()`.

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
  compilerWarning('Memory.printMemAllocs() is deprecated - please use ',
                  'Memory.Diagnostics.printMemAllocs()');

  pragma "insert line file info" 
  extern proc chpl_printMemAllocs(thresh);

  chpl_printMemAllocs(thresh);
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.printMemAllocsByType()`.

  Print summary information about allocated memory to ``memLog``.  The
  report contains a section for each top-level locale, containing a
  table of entries, one for each different allocation type for which
  at least one allocation exists on that locale.  The entries show the
  type (that is, the string the code requesting the allocation used to
  describe what it would store there) and the total number of
  allocations and bytes allocated for that type.
*/
proc printMemAllocsByType() {
  compilerWarning('Memory.printMemAllocsByType() is deprecated - ',
                  'please use Memory.Diagnostics.printMemAllocsByType()');

  pragma "insert line file info" 
  extern proc chpl_printMemAllocsByType();

  chpl_printMemAllocsByType();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.printMemAllocStats()`.

  Print summary memory statistics to ``memLog``.  The report contains
  a section for each top-level locale showing the number of bytes of
  memory currently allocated, the maximum number allocated at any
  point during execution (the high-water mark), and the sum of the
  sizes of all allocation and deallocation requests.
*/
proc printMemAllocStats() {
  compilerWarning('Memory.printMemAllocStats() is deprecated - ',
                  'please use Memory.Diagnostics.printMemAllocStats()');

  pragma "insert line file info"
  extern proc chpl_printMemAllocStats();

  chpl_printMemAllocStats();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.startVerboseMem()`.

  Start on-the-fly reporting of memory allocations and deallocations
  done on any locale.  Continue reporting until :proc:`stopVerboseMem`
  is called.

  The reporting output consists of a single line describing each
  memory allocation or deallocation, written to ``memLog``.
 */
proc startVerboseMem() { 
  compilerWarning('Memory.startVerboseMem() is deprecated - ',
                  'please use Memory.Diagnostics.startVerboseMem()');

  extern proc chpl_startVerboseMem();
  chpl_startVerboseMem();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.stopVerboseMem()`.

  Stop on-the-fly reporting of memory allocations and deallocations
  done on any locale.
 */
proc stopVerboseMem() {
  compilerWarning('Memory.stopVerboseMem() is deprecated - ',
                  'please use Memory.Diagnostics.stopVerboseMem()');

  extern proc chpl_stopVerboseMem();
  chpl_stopVerboseMem();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.startVerboseMemHere()`.

  Start on-the-fly reporting of memory allocations and deallocations
  done on this locale.  Continue reporting until
  :proc:`stopVerboseMemHere` is called.

  The reporting output consists of a single line describing each
  memory allocation or deallocation, written to ``memLog``.
 */
proc startVerboseMemHere() {
  compilerWarning('Memory.startVerboseMemHere() is deprecated - ',
                  'please use Memory.Diagnostics.startVerboseMemHere()');

  extern proc chpl_startVerboseMemHere();
  chpl_startVerboseMemHere();
}

/*
  .. warning::

    This function is deprecated - please use
    :proc:`Diagnostics.stopVerboseMemHere()`.

  Stop on-the-fly reporting of memory allocations and deallocations
  done on this locale.
 */
proc stopVerboseMemHere() {
  compilerWarning('Memory.stopVerboseMemHere() is deprecated - ',
                  'please use Memory.Diagnostics.stopVerboseMemHere()');

  extern proc chpl_stopVerboseMemHere();
  chpl_stopVerboseMemHere();
}

}
