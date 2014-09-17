/*
 * Copyright 2004-2014 Cray Inc.
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
pragma "no use ChapelStandard"
module MemTracking
{
  config const
    memTrack: bool = false,
    memStats: bool = false, 
    memLeaks: bool = false,
    memLeaksTable: bool = false,
    memMax: size_t = 0,
    memThreshold: size_t = 0,
    memLog: c_string = "";
  const s_memLog = toString(memLog);

  pragma "no auto destroy"
  config const
    memLeaksLog: c_string = "";
  const s_memLeaksLog = toString(memLeaksLog);

  //
  // This communicates the settings of the various memory tracking
  // config consts to the runtime code that actually implements the
  // memory tracking.
  //
  export
  proc chpl_memTracking_returnConfigVals(ref ret_memTrack: bool,
                                         ref ret_memStats: bool,
                                         ref ret_memLeaks: bool,
                                         ref ret_memLeaksTable: bool,
                                         ref ret_memMax: size_t,
                                         ref ret_memThreshold: size_t,
                                         ref ret_memLog: c_string,
                                         ref ret_memLeaksLog: c_string) {
    ret_memTrack = memTrack;
    ret_memStats = memStats;
    ret_memLeaks = memLeaks;
    ret_memLeaksTable = memLeaksTable;
    ret_memMax = memMax;
    ret_memThreshold = memThreshold;
    // The following is an abuse of a hack used to copy strings to
    // remote locales.  When strings are implemented as records,
    // XXX.locale.id should be replaced by 'here.id' as noted below.
    //
    // HACK: For some reason, when accessing a member field of a
    // string (.locale is the only such field), the string is copied
    // from the remote locale into a temporary.  Here we abuse that by
    // accessing the locale id.  The subsequent assignment to 'ts' is
    // from the compiler inserted temporary.
    if s_memLog.locale.id /* here.id */ != 0 {
      // Remote strings must be copied before accessing c_str()
      const ts = s_memLog;
      ret_memLog = ts.c_str();
    } else {
      ret_memLog = memLog;
    }
    if s_memLeaksLog.locale.id /* here.id */ != 0 {
      // Remote strings must be copied before accessing c_str()
      const ts = s_memLeaksLog;
      ret_memLeaksLog = ts.c_str();
    } else {
      ret_memLeaksLog = memLeaksLog;
    }
  }
}
