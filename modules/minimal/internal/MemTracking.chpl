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
                                         ref ret_memMax: uint(64),       // **
                                         ref ret_memThreshold: uint(64), // **
                                         ref ret_memLog: c_string,
                                         ref ret_memLeaksLog: c_string) {

    // ** In minimal-modules mode, I've hard-coded these size_t
    // arguments to uint(64) rather than using the size_t aliases
    // in SysCTypes.chpl because doing that requires dragging in a
    // bunch of other ChapelBase code.  My assumption here is that
    // size_t will either be, or be compatible with, uint(64) for
    // most developers.  If that turns out not to be the case, we
    // can reconsider this choice.
  }
}
