/*
 * Copyright 2004-2020 Cray Inc.
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

// MemTracking.chpl
//
module MemTracking
{
  private use ChapelStandard, SysCTypes;

  config const
    memTrack: bool = false,
    memStats: bool = false,
    memLeaksByType: bool = false,
    memLeaks: bool = false,
    memMax: uint = 0,
    memThreshold: uint = 0,
    memLog: string;

  pragma "no auto destroy"
  config const
    memLeaksLog: string;

  /* Causes the contents of the memory tracking array to be printed at the end
     of the program.
     Entries remaining in the memory tracking array represent leaked memory,
     because they are tracked allocations with no corresponding free.

     The dump is performed only if the --memLeaksByDesc option is present and has
     a string argument.
       --memLeaksByDesc="" causes all memory records to be printed.  Same as --memLeaks.
       --memLeaksByDesc="<alloc-type-string>" causes only those memory records
         matching the given <alloc-type-string> to be printed.
     For example, --memLeaksByDesc="string copy data" causes only string copy
     data leaks to be printed.
  */
  pragma "no auto destroy"
  config const
    memLeaksByDesc: string;

  // Safely cast to size_t instances of memMax and memThreshold.
  const cMemMax = memMax.safeCast(size_t),
    cMemThreshold = memThreshold.safeCast(size_t);

  //
  // This communicates the settings of the various memory tracking
  // config consts to the runtime code that actually implements the
  // memory tracking.
  //
  // This function is a little tricky as it is called from every
  // locale from the runtime.  Recall that c_string is considered a
  // local-only data type, so we must use some tricks to copy the
  // c_string from locale 0 to the remote locales.  We use the globals
  // s_memLog and s_memLeaksLog to create global Chapel strings to
  // make them available to all locales.
  //
  export
  proc chpl_memTracking_returnConfigVals(ref ret_memTrack: bool,
                                         ref ret_memStats: bool,
                                         ref ret_memLeaksByType: bool,
                                         ref ret_memLeaksByDesc: c_string,
                                         ref ret_memLeaks: bool,
                                         ref ret_memMax: size_t,
                                         ref ret_memThreshold: size_t,
                                         ref ret_memLog: c_string,
                                         ref ret_memLeaksLog: c_string) {
    ret_memTrack = memTrack;
    ret_memStats = memStats;
    ret_memLeaksByType = memLeaksByType;
    ret_memLeaks = memLeaks;
    ret_memMax = cMemMax;
    ret_memThreshold = cMemThreshold;

    if (here.id != 0) {
      if memLeaksByDesc.length != 0 {
        var local_memLeaksByDesc = memLeaksByDesc;
        // Intentionally leak the string to persist the underlying buffer
        local_memLeaksByDesc.isowned = false;
        ret_memLeaksByDesc = local_memLeaksByDesc.c_str();
      } else {
        ret_memLeaksByDesc = nil;
      }

      if memLog.length != 0 {
        var local_memLog = memLog;
        // Intentionally leak the string to persist the underlying buffer
        local_memLog.isowned = false;
        ret_memLog = local_memLog.c_str();
      } else {
        ret_memLog = nil;
      }

      if memLeaksLog.length != 0 {
        var local_memLeaksLog = memLeaksLog;
        // Intentionally leak the string to persist the underlying buffer
        local_memLeaksLog.isowned = false;
        ret_memLeaksLog = local_memLeaksLog.c_str();
      } else {
        ret_memLeaksLog = nil;
      }

     } else {
      ret_memLeaksByDesc = memLeaksByDesc.c_str();
      ret_memLog = memLog.c_str();
      ret_memLeaksLog = memLeaksLog.c_str();
    }
  }
}
