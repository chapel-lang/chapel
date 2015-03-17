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

/* This module provides for collecting and reporting statistics about
   communication operations between network-connected locales.  The
   operations include various kinds of remote reads (GETs), remote
   writes (PUTs), and remote forks.  Callers can request on-the-fly
   output each time a remote operation occurs, or aggregate counting of
   the operations for later retrieval.  The former gives more detailed
   information but has much more overhead.  The latter has much less
   overhead but only provides aggregate information.

   All forms of statistics collection are done between pairs of function
   calls that turn it on and off.  On-the-fly reporting across all
   locales is done like this::

     startVerboseComm();
     // between start/stop calls, report comm ops initiated on any locale
     stopVerboseComm();

   On-the-fly reporting for a single locale is done like this::

     startVerboseCommHere();
     // between start/stop calls, report comm ops initiated on this locale
     stopVerboseCommHere();

   Aggregate statistics collection requires a few more calls.  One has
   to retrieve the statistics after they are collected and, if they are
   to be used again, reset the internal counters afterward.  Collecting
   statistics across all locales is done like this::

     // (optional) if we counted previously, reset the counters to zero
     resetCommDiagnostics();
     startCommDiagnostics();
     // between start/stop calls, count comm ops initiated on any locale
     stopCommDiagnostics();
     // retrieve the counts and report the results
     writeln(getCommDiagnostics());

   Collecting statistics on just the calling locale is much the same.
   Only the procedure names change::

     // (optional) if we counted previously, reset the counters to zero
     resetCommDiagnosticsHere();
     startCommDiagnosticsHere();
     // between start/stop calls, count comm ops initiated on this locale
     stopCommDiagnosticsHere();
     // retrieve the counts and report the results
     writeln(getCommDiagnosticsHere());

   The optional call to reset the counters is only needed when a program
   collects communication statistics more than once.  In this case, the
   counters have to be set back to zero before starting the second and
   succeeding collection periods.  By far the most common situation is
   that programs only collect the communication counters once per run,
   in which case this step is not needed.

   Note that the same internal mechanisms and counters are used for
   counting on all locales and counting on just the calling locale, so
   trying to do both at once may lead to surprising turn-of/turn-off
   behavior and/or incorrect results.
 */
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

  /* Aggregated communication operation counts.  This record type is
     defined in the same way by both the runtime and this module,
     because we don't have a good way to inherit types back and forth
     between the two.  This first definition duplicates the one in the
     runtime.
   */
  extern record chpl_commDiagnostics {
    /*
      blocking GETs, in which initiator waits for completion
     */
    var get: uint(64);
    /*
      non-blocking GETs
     */
    var get_nb: uint(64);
    /*
      blocking PUTs, in which initiator waits for completion
     */
    var put: uint(64);
    /*
      non-blocking PUTs
     */
    var put_nb: uint(64);
    /*
      tests for non-blocking GET/PUT completions
     */
    var test_nb: uint(64);
    /*
      blocking waits for non-blocking GET/PUT completions
     */
    var wait_nb: uint(64);
    /*
      non-blocking waits for non-blocking GET/PUT completions
     */
    var try_nb: uint(64);
    /*
      blocking remote executions, in which initiator waits for completion
     */
    var fork: uint(64);
    /*
      blocking remote executions performed by the target locale's
      Active Message handler
     */
    var fork_fast: uint(64);
    /*
      non-blocking remote executions
     */
    var fork_nb: uint(64);
  };

  /*
    The Chapel record type inherits the runtime's definition of it.
   */
  type commDiagnostics = chpl_commDiagnostics;

  pragma "no doc"
  extern proc chpl_startVerboseComm();

  pragma "no doc"
  extern proc chpl_stopVerboseComm();

  pragma "no doc"
  extern proc chpl_startVerboseCommHere();

  pragma "no doc"
  extern proc chpl_stopVerboseCommHere();

  pragma "no doc"
  extern proc chpl_gen_startCommDiagnostics();

  pragma "no doc"
  extern proc chpl_gen_stopCommDiagnostics();

  pragma "no doc"
  extern proc chpl_gen_startCommDiagnosticsHere();

  pragma "no doc"
  extern proc chpl_gen_stopCommDiagnosticsHere();

  pragma "no doc"
  extern proc chpl_resetCommDiagnosticsHere();

  pragma "no doc"
  extern proc chpl_getCommDiagnosticsHere(out cd: commDiagnostics);

  /*
    Start real-time reporting of communication initiated on any locale.
    The output consists of a line written to ``stdout`` for each
    communication operation.  (Here ``stdout`` means the file associated
    with the process, not the Chapel channel with the same name.)
   */
  proc startVerboseComm() { chpl_startVerboseComm(); }

  /*
    Stop real-time reporting of communication initiated on any locale.
   */
  proc stopVerboseComm() { chpl_stopVerboseComm(); }

  /*
    Start real-time reporting of communication initiated on this locale.
    The output consists of a line written to ``stdout`` for each
    communication operation.  (Here ``stdout`` means the file associated
    with the process, not the Chapel channel with the same name.)
   */
  proc startVerboseCommHere() { chpl_startVerboseCommHere(); }

  /*
    Stop real-time reporting of communication initiated on this locale.
   */
  proc stopVerboseCommHere() { chpl_stopVerboseCommHere(); }

  /*
    Start counting communication operations across the whole program.
   */
  proc startCommDiagnostics() {
    chpl_gen_startCommDiagnostics();
  }

  /*
    Stop counting communication operations across the whole program.
   */
  proc stopCommDiagnostics() {
    chpl_gen_stopCommDiagnostics();
  }

  /*
    Start counting communication operations initiated on the calling locale.
   */
  proc startCommDiagnosticsHere() {
    chpl_gen_startCommDiagnosticsHere();
  }

  /*
    Stop counting communication operations initiated on the calling locale.
   */
  proc stopCommDiagnosticsHere() {
    chpl_gen_stopCommDiagnosticsHere();
  }

  /*
    Reset aggregate communication counts across the whole program.
   */
  proc resetCommDiagnostics() {
    for loc in Locales do on loc do
      resetCommDiagnosticsHere();
  }

  /*
    Reset aggregate communication counts on the calling locale.
   */
  inline proc resetCommDiagnosticsHere() {
    chpl_resetCommDiagnosticsHere();
  }

  // See note above regarding extern records
  pragma "no doc"
  extern proc chpl_numCommGets(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommNBGets(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommPuts(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommNBPuts(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommTestNB(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommWaitNB(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommTryNB(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommForks(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommFastForks(): uint(64);

  pragma "no doc"
  extern proc chpl_numCommNBForks(): uint(64);

  /*
    Retrieve aggregate communication counts for the whole program.

    :returns: array of counts of comm ops initiated on each locale
    :rtype: `[LocaleSpace] commDiagnostics`
   */
  proc getCommDiagnostics() {
    var D: [LocaleSpace] commDiagnostics;
    for loc in Locales do on loc {
      // See note above regarding extern records
      D(loc.id).get = chpl_numCommGets();
      D(loc.id).get_nb = chpl_numCommNBGets();
      D(loc.id).put = chpl_numCommPuts();
      D(loc.id).put_nb = chpl_numCommNBPuts();
      D(loc.id).test_nb = chpl_numCommTestNB();
      D(loc.id).wait_nb = chpl_numCommWaitNB();
      D(loc.id).try_nb = chpl_numCommTryNB();
      D(loc.id).fork = chpl_numCommForks();
      D(loc.id).fork_fast = chpl_numCommFastForks();
      D(loc.id).fork_nb = chpl_numCommNBForks();
    }
    return D;
  }

  /*
    Retrieve aggregate communication counts for this locale.

    :returns: counts of comm ops initiated on the calling locale
    :rtype: `commDiagnostics`
   */
  proc getCommDiagnosticsHere() {
    var cd: commDiagnostics;
    cd.get = chpl_numCommGets();
    cd.get_nb = chpl_numCommNBGets();
    cd.put = chpl_numCommPuts();
    cd.put_nb = chpl_numCommNBPuts();
    cd.test_nb = chpl_numCommTestNB();
    cd.wait_nb = chpl_numCommWaitNB();
    cd.try_nb = chpl_numCommTryNB();
    cd.fork = chpl_numCommForks();
    cd.fork_fast = chpl_numCommFastForks();
    cd.fork_nb = chpl_numCommNBForks();
    return cd;
  }

}
