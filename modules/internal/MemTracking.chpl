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

// MemTracking.chpl
//
module MemTracking
{
  //TODO strings: super broken now
  config const
    memTrack: bool = false,
    memStats: bool = false,
    memLeaksByType: bool = false,
    memLeaks: bool = false,
    memMax: uint = 0,
    memThreshold: uint = 0,
    memLog: c_ptr(uint(8)) = nil;

  pragma "no auto destroy"
  config const
    memLeaksLog: c_ptr(uint(8)) = nil;

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
    memLeaksByDesc: c_string = "";

  // Safely cast to size_t instances of memMax and memThreshold.
  const cMemMax = memMax.safeCast(size_t),
    cMemThreshold = memThreshold.safeCast(size_t);

  // Globally accessible copy of the corresponding c_string consts
  //const s_memLog: string = memLog;
  //const s_memLeaksLog: string = memLeaksLog;
  const s_memLog: string;
  const s_memLeaksLog: string;

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
                                         ref ret_memLog: c_ptr(uint(8)),
                                         ref ret_memLeaksLog: c_ptr(uint(8))) {
    ret_memTrack = memTrack;
    ret_memStats = memStats;
    ret_memLeaksByType = memLeaksByType;
    ret_memLeaks = memLeaks;
    ret_memMax = cMemMax;
    ret_memThreshold = cMemThreshold;

    if (here.id != 0) {
      // These c_strings are going to be leaked
      if s_memLeaksByDesc.len != 0 then
        ret_memLeaksByDesc = remoteStringCopy(s_memLeaksByDesc.home.id,
                                      s_memLeaksByDesc.base,
                                      s_memLeaksByDesc.len);
      else ret_memLeaksByDesc = "";
      if s_memLog.len != 0 then
        ret_memLog = copyRemoteBuffer(s_memLog.locale.id,
                                      s_memLog.buff,
                                      s_memLog.len);
      else ret_memLog = nil;
      if s_memLeaksLog.len != 0 then
        ret_memLeaksLog = copyRemoteBuffer(s_memLeaksLog.locale.id,
                                           s_memLeaksLog.buff,
                                           s_memLeaksLog.len);
      else ret_memLeaksLog = nil;
    } else {
      ret_memLeaksByDesc = memLeaksByDesc;
      ret_memLog = memLog;
      ret_memLeaksLog = memLeaksLog;
    }
  }
}
