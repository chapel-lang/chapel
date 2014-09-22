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

// CommDiagnostics.chpl
//
pragma "no use ChapelStandard"
module CommDiagnostics
{
  //
  // multi-locale diagnostics/debugging support
  //
  
  // There should be a type like this declared in chpl-comm.h with a single
  // function that returns the C struct.  We're not doing it that way yet
  // due to some shortcomings in our extern records implementation.
  // Once that gets sorted out, we can turn this into an extern record,
  // and remove the 8 or so individual functions below that return the
  // various counters.
  extern record chpl_commDiagnostics {
    var get: uint(64);
    var get_nb: uint(64);
    var get_nb_test: uint(64);
    var get_nb_wait: uint(64);
    var put: uint(64);
    var fork: uint(64);
    var fork_fast: uint(64);
    var fork_nb: uint(64);
  };
  
  type commDiagnostics = chpl_commDiagnostics;
  
  extern proc chpl_startVerboseComm();
  extern proc chpl_stopVerboseComm();
  extern proc chpl_startVerboseCommHere();
  extern proc chpl_stopVerboseCommHere();
  extern proc chpl_gen_startCommDiagnostics();
  extern proc chpl_gen_stopCommDiagnostics();
  extern proc chpl_gen_startCommDiagnosticsHere();
  extern proc chpl_gen_stopCommDiagnosticsHere();
  extern proc chpl_resetCommDiagnosticsHere();
  extern proc chpl_getCommDiagnosticsHere(out cd: commDiagnostics);
  
  proc startVerboseComm() { chpl_startVerboseComm(); }
  proc stopVerboseComm() { chpl_stopVerboseComm(); }
  proc startVerboseCommHere() { chpl_startVerboseCommHere(); }
  proc stopVerboseCommHere() { chpl_stopVerboseCommHere(); }
  
  proc startCommDiagnostics() {
    chpl_gen_startCommDiagnostics();
  }
  proc stopCommDiagnostics() {
    chpl_gen_stopCommDiagnostics();
  }
  proc startCommDiagnosticsHere() {
    chpl_gen_startCommDiagnosticsHere();
  }
  proc stopCommDiagnosticsHere() {
    chpl_gen_stopCommDiagnosticsHere();
  }
  
  proc resetCommDiagnostics() {
    for loc in Locales do on loc do
      resetCommDiagnosticsHere();
  }
  
  inline proc resetCommDiagnosticsHere() {
    chpl_resetCommDiagnosticsHere();
  }
  
  // See note above regarding extern records
  extern proc chpl_numCommGets(): uint(64);
  extern proc chpl_numCommNBGets(): uint(64);
  extern proc chpl_numCommTestNBGets(): uint(64);
  extern proc chpl_numCommWaitNBGets(): uint(64);
  extern proc chpl_numCommPuts(): uint(64);
  extern proc chpl_numCommForks(): uint(64);
  extern proc chpl_numCommFastForks(): uint(64);
  extern proc chpl_numCommNBForks(): uint(64);
  
  proc getCommDiagnostics() {
    var D: [LocaleSpace] commDiagnostics;
    for loc in Locales do on loc {
      // See note above regarding extern records
      D(loc.id).get = chpl_numCommGets();
      D(loc.id).put = chpl_numCommPuts();
      D(loc.id).fork = chpl_numCommForks();
      D(loc.id).fork_fast = chpl_numCommFastForks();
      D(loc.id).fork_nb = chpl_numCommNBForks();
      D(loc.id).get_nb = chpl_numCommNBGets();
      D(loc.id).get_nb_test = chpl_numCommTestNBGets();
      D(loc.id).get_nb_wait = chpl_numCommWaitNBGets();
    }
    return D;
  }
  
  proc getCommDiagnosticsHere() {
    var cd: commDiagnostics;
    cd.get = chpl_numCommGets();
    cd.put = chpl_numCommPuts();
    cd.fork = chpl_numCommForks();
    cd.fork_fast = chpl_numCommFastForks();
    cd.fork_nb = chpl_numCommNBForks();
    cd.get_nb = chpl_numCommNBGets();
    cd.get_nb_test = chpl_numCommTestNBGets();
    cd.get_nb_wait = chpl_numCommWaitNBGets();
    return cd;
  }
  
}
