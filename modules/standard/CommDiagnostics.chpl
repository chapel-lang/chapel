/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  This module provides support for reporting and counting
  communication operations between network-connected locales.  The
  operations include various kinds of remote reads (GETs), remote
  writes (PUTs), and remote executions.
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
      on Locales(1) {     // should execute_on a blocking task onto locale 1
        x = x + 1;        // should invoke a remote put and a remote get
      }
      stopVerboseComm();
    }

  Executing this on two locales with the ``-nl 2`` command line
  option results in the following output::

    0: remote task created on 1
    1: t.chpl:6: remote get from 0, 8 bytes
    1: t.chpl:6: remote put to 0, 8 bytes

  The initial number refers to the locale reporting the communication
  event.  The file name and line number point to the place in the
  code that triggered the communication event.  (For remote execute_ons,
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
      on Locales(1) {     // should execute_on a blocking task onto locale 1
        x = x + 1;        // should invoke a remote put and a remote get
      }
      stopCommDiagnostics();
      writeln(getCommDiagnostics());
    }

  Executing this on two locales with the ``-nl 2`` command line
  option results in the following output::

    (execute_on = 1) (get = 1, put = 1)

  The first parenthesized group contains the counts for locale 0, and
  the second contains the counts for locale 1.  So, for the
  instrumented section of this program we can say that a remote execute_on
  was executed on locale 0, and a remote get and a remote put were
  executed on locale 1.

  **Studying Communication During Module Initialization**

  It is hard for a programmer to determine exactly what happens during
  initialization or teardown of a module, because the code that runs
  then does so only implicitly, as a result of the declarations
  present.  And even if that code can be identified, doing debug
  output or logging data for later reporting might not work because
  the Chapel capabilities needed to do so could be unavailable due to
  being implemented by built-in modules which themselves are not yet
  initialized, or have already been torn down.

  To help with that problem, this module provides built-in support for
  studying communication operations during module initialization and
  teardown.  To use it, set either or both of the config params
  :param:`printInitVerboseComm` and :param:`printInitCommCounts`,
  described below.  You can do this by using appropriate
  ``-sconfigParamName=value`` command line options when you compile
  your program.

  The reporting and/or counting enabled by these covers all of program
  execution, from just before the first module is initialized until
  just after the last one is torn down.  This is almost always a
  superset of the part of the program that is of interest, which is
  often just a single module.  To learn what communication is being
  done by a single module during its initialization and teardown it is
  often necessary to run a small test program twice, once with that
  module present and once without it.
 */
module CommDiagnostics
{
  /*
    If this is `false`, a written `commDiagnostics` value does not
    include "unstable" fields even when they are non-zero.  Unstable
    fields are those expected to have unpredictable values for multiple
    executions of the same code sequence.  Setting this to `true` causes
    such fields, if non-zero, to be included when a `commDiagnostics`
    value is written.  At present the only unstable field is the `amo`
    counter, whose instability is due to the use of atomic reads in spin
    loops that wait for parallelism and on-statements to complete.
   */
  config param commDiagsPrintUnstable = false;

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
      atomic memory operations
     */
    var amo: uint(64);
    /*
      blocking remote executions, in which initiator waits for completion
     */
    var execute_on: uint(64);
    /*
      blocking remote executions performed by the target locale's
      Active Message handler
     */
    var execute_on_fast: uint(64);
    /*
      non-blocking remote executions
     */
    var execute_on_nb: uint(64);

    proc writeThis(c) throws {
      use Reflection;

      var first = true;
      c <~> "(";
      for param i in 0..<numFields(chpl_commDiagnostics) {
        param name = getFieldName(this.type, i);
        const val = getField(this, i);
        if val != 0 {
          if commDiagsPrintUnstable || name != 'amo' {
            if first then first = false; else c <~> ", ";
            c <~> name <~> " = " <~> val;
          }
        }
      }
      if first then c <~> "<no communication>";
      c <~> ")";
    }
  };

  /*
    The Chapel record type inherits the comm layer definition of it.
   */
  type commDiagnostics = chpl_commDiagnostics;

  private extern proc chpl_comm_startVerbose(print_unstable: bool);

  private extern proc chpl_comm_stopVerbose();

  private extern proc chpl_comm_startVerboseHere(print_unstable: bool);

  private extern proc chpl_comm_stopVerboseHere();

  private extern proc chpl_comm_startDiagnostics(print_unstable: bool);

  private extern proc chpl_comm_stopDiagnostics();

  private extern proc chpl_comm_startDiagnosticsHere(print_unstable: bool);

  private extern proc chpl_comm_stopDiagnosticsHere();

  private extern proc chpl_comm_resetDiagnosticsHere();

  private extern proc chpl_comm_getDiagnosticsHere(out cd: commDiagnostics);

  /*
    Start on-the-fly reporting of communication initiated on any locale.
   */
  proc startVerboseComm() {
    chpl_comm_startVerbose(commDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of communication initiated on any locale.
   */
  proc stopVerboseComm() { chpl_comm_stopVerbose(); }

  /*
    Start on-the-fly reporting of communication initiated on this locale.
   */
  proc startVerboseCommHere() {
    chpl_comm_startVerboseHere(commDiagsPrintUnstable);
  }

  /*
    Stop on-the-fly reporting of communication initiated on this locale.
   */
  proc stopVerboseCommHere() { chpl_comm_stopVerboseHere(); }

  /*
    Start counting communication operations across the whole program.
   */
  proc startCommDiagnostics() {
    chpl_comm_startDiagnostics(commDiagsPrintUnstable);
  }

  /*
    Stop counting communication operations across the whole program.
   */
  proc stopCommDiagnostics() {
    chpl_comm_stopDiagnostics();
  }

  /*
    Start counting communication operations initiated on this locale.
   */
  proc startCommDiagnosticsHere() {
    chpl_comm_startDiagnosticsHere(commDiagsPrintUnstable);
  }

  /*
    Stop counting communication operations initiated on this locale.
   */
  proc stopCommDiagnosticsHere() {
    chpl_comm_stopDiagnosticsHere();
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
    chpl_comm_resetDiagnosticsHere();
  }

  /*
    Retrieve aggregate communication counts for the whole program.

    :returns: array of counts of comm ops initiated on each locale
    :rtype: `[LocaleSpace] commDiagnostics`
   */
  proc getCommDiagnostics() {
    var D: [LocaleSpace] commDiagnostics;
    for loc in Locales do on loc {
      D(loc.id) = getCommDiagnosticsHere();
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
    chpl_comm_getDiagnosticsHere(cd);
    return cd;
  }


  /*
    If this is set, on-the-fly reporting of communication operations
    will be turned on before any module initialization begins and
    turned off after all module teardown ends.  See procedures
    :proc:`startVerboseComm` and :proc:`stopVerboseComm` for more
    information.
   */
  config param printInitVerboseComm = false;

  /*
    If this is set, communication operations are counted from before
    any module initialization begins until after all module teardown
    ends, and then the aggregate counts are printed.  See procedures
    :proc:`startCommDiagnostics`, :proc:`stopCommDiagnostics`, and
    :proc:`getCommDiagnostics` for more information.
   */
  config param printInitCommCounts = false;

}
