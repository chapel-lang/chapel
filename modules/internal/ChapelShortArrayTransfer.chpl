/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

// This is a helper module to support the Short Array Transfer optimization.
// For array-to-array assignments where number of elements (or maybe the data
// size?) are below some threshold, just looping serially is more efficient
// than both parallel iteration and memcpy.

module ChapelShortArrayTransfer {
  use ChapelBase;
  use ChapelDebugPrint only chpl_debug_writeln;
  use ChplConfig only CHPL_LOCALE_MODEL;

  @chpldoc.nodoc
  config param debugShortArrayTransfer = false;
  @chpldoc.nodoc
  config param disableShortArrayTransfer = false;
  @chpldoc.nodoc
  config const shortArrayTransferThreshold = 50;



  proc chpl__staticCheckShortArrayTransfer(a, b) param {
    // this is the case I'm focusing on in the initial PR. This can definitely
    // be loosened up... by a lot.
    return !disableShortArrayTransfer && isProtoSlice(a) && isProtoSlice(b);
  }

  inline proc chpl__dynamicCheckShortArrayTransfer(a, b) {
    param localCompilation = _local && CHPL_LOCALE_MODEL=="flat";
    const sizeOk = a.sizeAs(uint) < shortArrayTransferThreshold;

    if debugShortArrayTransfer {
      chpl_debug_writeln("<ShortArrayTransfer> Size: ", a.sizeAs(uint),
                         " Threshold: ", shortArrayTransferThreshold);
      if sizeOk then
        chpl_debug_writeln("<ShortArrayTransfer> size qualifies");
      else
        chpl_debug_writeln("<ShortArrayTransfer> size doesn't qualify");
    }

    if localCompilation {
      return sizeOk;
    }
    else {
      // No `.locale` to avoid overheads. Note that this is an optimization for
      // fast-running code. Small things matter.
      const bothLocal = chpl__bothLocal(a, b);
      if debugShortArrayTransfer {
        if bothLocal then
          chpl_debug_writeln("<ShortArrayTransfer> locality qualifies");
        else
          chpl_debug_writeln("<ShortArrayTransfer> locality does not qualify");
      }

      return sizeOk && bothLocal;
    }
  }
}
