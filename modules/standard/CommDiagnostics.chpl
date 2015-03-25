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

pragma "no use ChapelStandard"

/*
  This module provides support for reporting and counting
  communication operations between network-connected locales.  The
  operations include various kinds of remote reads (GETs), remote
  writes (PUTs), and remote forks (which are actually more like remote
  procedure calls, and are used to implement ``on`` statements).
  Callers can request on-the-fly output each time a remote operation
  occurs, or count such operations as they occur and retrieve the
  counts later.  The former gives more detailed information but has
  much more overhead.  The latter has much less overhead but only
  provides aggregate information.

  **On-the-fly Reporting**

  All forms of communication reporting and counting are done between
  pairs of function calls that turn it on and off.  On-the-fly
  reporting across all locales is done like this::

    startVerboseComm();
    // between start/stop calls, report comm ops initiated on any locale
    stopVerboseComm();

  On-the-fly reporting for just the calling locale is similar.  Only
  the procedure names change::

    startVerboseCommHere();
    // between start/stop calls, report comm ops initiated on this locale
    stopVerboseCommHere();

  In either case, the output produced consists of a line written to
  ``stdout`` for each communication operation.  (Here ``stdout`` means
  the file associated with the process, not the Chapel channel with the
  same name.)

  Consider this little example program::

    use CommDiagnostics;
    proc main() {
      startVerboseComm();
      var x: int = 1;
      on Locales(1) {     // should fork a blocking task onto locale 1
        x = x + 1;        // should invoke a remote put and a remote get
      }
      stopVerboseComm();
    }

  Executing this on two locales with the ``-nl 2`` command line
  option results in the following output::

    0: remote task created on 1
    1: t.chpl:6: remote get from 0
    1: t.chpl:6: remote put to 0

  The initial number refers to the locale reporting the communication
  event.  The file name and line number point to the place in the
  code that triggered the communication event.  (For remote forks,
  file name and line number information is not yet reported.)

  **Counting Communication Operations**

  Counting communication operations requires a few more calls then just
  reporting them does.  In particular, the counts have to be retrieved
  after they are collected and, if they have been used previously, the
  internal counters have to be reset before counting is turned on.
  Counting across all locales is done like this::

    // (optional) if we counted previously, reset the counters to zero
    resetCommDiagnostics();
    startCommDiagnostics();
    // between start/stop calls, count comm ops initiated on any locale
    stopCommDiagnostics();
    // retrieve the counts and report the results
    writeln(getCommDiagnostics());

  Counting on just the calling locale is similar.  Just as for
  on-the-fly reporting, only the procedure names change::

    // (optional) if we counted previously, reset the counters to zero
    resetCommDiagnosticsHere();
    startCommDiagnosticsHere();
    // between start/stop calls, count comm ops initiated on this locale
    stopCommDiagnosticsHere();
    // retrieve the counts and report the results
    writeln(getCommDiagnosticsHere());

  The optional call to reset the counters is only needed when a program
  collects counts more than once.  In this case, the counters have to
  be set back to zero before starting the second and succeeding
  counting periods.  By far the most common situation is that programs
  only collect communication counts once per run, in which case this
  step is not needed.

  Note that the same internal mechanisms and counters are used for
  counting on all locales and counting on just the calling locale, so
  trying to do both at once may lead to surprising turn-on/turn-off
  behavior and/or incorrect results.

  Consider this little example program::

    use CommDiagnostics;
    proc main() {
      startCommDiagnostics();
      var x: int = 1;
      on Locales(1) {     // should fork a blocking task onto locale 1
        x = x + 1;        // should invoke a remote put and a remote get
      }
      stopCommDiagnostics();
      writeln(getCommDiagnostics());
    }

  Executing this on two locales with the ``-nl 2`` command line
  option results in the following output::

    (get = 0, get_nb = 0, put = 0, put_nb = 0, test_nb = 0, wait_nb = 0, try_nb = 0, fork = 1, fork_fast = 0, fork_nb = 0) (get = 1, get_nb = 0, put = 1, put_nb = 0, test_nb = 0, wait_nb = 0, try_nb = 0, fork = 0, fork_fast = 0, fork_nb = 0)

  The first parenthesized group contains the counts for locale 0, and
  the second contains the counts for locale 1.  So, for the
  instrumented section of this program we can say that a remote fork
  was executed on locale 0, and a remote get and a remote put were
  executed on locale 1.
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
     defined in the same way by both the underlying comm layer(s) and
     this module, because we don't have a good way to inherit types back
     and forth between the two.  This first definition duplicates the
     one in the comm layer(s).
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
    The Chapel record type inherits the comm layer definition of it.
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
    Start on-the-fly reporting of communication initiated on any locale.
   */
  proc startVerboseComm() { chpl_startVerboseComm(); }

  /*
    Stop on-the-fly reporting of communication initiated on any locale.
   */
  proc stopVerboseComm() { chpl_stopVerboseComm(); }

  /*
    Start on-the-fly reporting of communication initiated on this locale.
   */
  proc startVerboseCommHere() { chpl_startVerboseCommHere(); }

  /*
    Stop on-the-fly reporting of communication initiated on this locale.
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
    Start counting communication operations initiated on this locale.
   */
  proc startCommDiagnosticsHere() {
    chpl_gen_startCommDiagnosticsHere();
  }

  /*
    Stop counting communication operations initiated on this locale.
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

    :returns: counts of comm ops initiated on this locale
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
