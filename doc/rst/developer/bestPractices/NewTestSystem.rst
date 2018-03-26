.. _readme-testsystem:

=====================
Chapel Testing System
=====================

The Chapel testing system is a key piece of technology for the Chapel
developer.  We use it as a harness for doing test-driven development,
for performing sanity checks on code before committing it, for bug and
issue tracking, and for nightly correctness and performance regression
testing.  Getting really comfortable with it is one of the most
important things a developer can do early in the development cycle.

The tests for the testing system are located in ``$CHPL_HOME/test``.
The main script that drives the test system itself is
``$CHPL_HOME/util/start_test``, though it relies on several helper scripts
located in ``$CHPL_HOME/util/test``.

This document provides only a high-level introduction to the testing
system.  For further details, ask a core Chapel developer for
suggestions.  You can also get a sense for the test system by looking
through the test directory itself to see how it is used in practice.

Outline
=======

 * `Summary of Testing Files`_
 * `So you want to make:`_

   - `a correctness test`_

     - `that relies on outside arguments or settings`_
     - `and control how and where it runs`_

       - `Controlling timeouts`_
       - `Different settings, different output`_

         - `Test should be limited to certain settings`_
         - `Behavior in all settings is appropriate, but varies`_

   - `a performance test`_

     - `Identifying Performance Keys`_
     - `Validating Performance Test Output`_
     - `Accumulating Performance Data in .dat files`_
     - `Other Performance Testing Options`_
     - `Comparing Multiple Versions`_
     - `Creating a graph comparing multiple variations`_
     - `Test Your Test Before Submitting`_

   - `a test that tracks a failure`_

     - `that requires Chapel developer effort to fix`_
     - `that should fix itself when a dependency updates`_

* `Invoking start_test for`_

  - `correctness testing`_
  - `performance testing`_


Summary of Testing Files
========================

.. TODO: When we move these docs to Sphinx, add :ref:'s to other parts of file,
         within this table

The following table serves as a quick reference for the various test files, and
as a table of contents for this page.
Using file base name, ``foo`` for the filenames in this table.

=================   ===========================================================
File                Contents of file
=================   ===========================================================
**correctness**
-------------------------------------------------------------------------------
foo.chpl            Chapel test program to compile and run
foo.test.c          C test program to compile and run
foo.good            expected output of test program
..
-------------------------------------------------------------------------------
**Test Settings**
-------------------------------------------------------------------------------
foo.compopts        line separated compiler flag configurations
COMPOPTS            directory-wide compiler flags
foo.execopts        line separated runtime flag configurations
EXECOPTS            directory-wide runtime flags
foo.execenv         line separated list of environment variables settings
EXECENV             directory-wide environment variables
foo.numlocales      number of locales to use in multi-locale run
..
-------------------------------------------------------------------------------
**TODO NAME ME**
-------------------------------------------------------------------------------
foo.catfiles        files to include when validating the expected output
foo.prediff         script that is run on the test output, before taking the
                    diff between the output and .good file
PREDIFF             directory-wide script that is run over test output
foo.precomp         script that is run prior to compilation of the test program
PRECOMP             directory-wide script that is run prior to compilation
foo.preexec         script that is run prior to execution of the test program
PREEXEC             directory-wide script that is run prior to execution
..
-------------------------------------------------------------------------------
**Testing System Settings**
-------------------------------------------------------------------------------
foo.noexec          empty file. Indicates .chpl file should only be compiled,
                    not executed.
foo.notest          empty file. Indicates the file should not be run explicitly
NOTEST              empty file. Indicates the directory should not be run
foo.skipif          line separated list of conditions under which the test
                    should not be run, or a script to compute the same
SKIPIF              same as above, but applied to the entire directory
foo.suppressif      line separated list of conditions under which the test is
                    expected to fail, or a script to compute the same
foo.timeout         time in seconds after which start_test should stop this test
..
-------------------------------------------------------------------------------
**performance**
-------------------------------------------------------------------------------
foo.perfcompopts    compiler flags, overrides .compopts for --performance
foo.perfexecopts    runtime flags, overrides .execopts for --performance
foo.perfexecenv     environment variables, overrides .execenv for --performance
foo.perfnumtrials   number of execution trials to run if no timeout specified
foo.perftimeout     time in seconds after which start_test should stop this test
foo.perfkeys        keys to search for in the output
foo.graph           Specifies which data files and perfkeys to graph, and
                    contains meta-data associated with labeling data sets,
                    axis, and graphs
test/GRAPHFILES     Acts as an index that tracks all .graph that should be
                    graphed.
..
-------------------------------------------------------------------------------
**futures**
-------------------------------------------------------------------------------
foo.future          Describes the future being tested, following the
                    newline-separated format of:
                    *category*, *title*, *issue #*
foo.bad             output generated on a failing test, to track if a known
                    failing future begins failing a different way
..
=================   ===========================================================


So you want to make:
====================

a correctness test
------------------

Though trivial, this test is available at ``$CHPL_HOME/test/Samples/Correctness``
in the Chapel source repository

A simplest use of the test system is to create a ``.chpl`` file containing
some Chapel code and a ``.good`` file containing the expected output.  For
example, given a directory containing two files:


``hi.chpl``

  .. code-block:: chapel

    writeln("Hi!");

``hi.good``

  .. code-block::  text

    Hi!


The test system can be exercised by invoking:

  ``start_test hi.chpl``

This is assuming ``$CHPL_HOME/util/`` is in the user's `$PATH`, which is
taken care of when sourcing ``$CHPL_HOME/util/setchplenv.bash``.

This will cause the compiler to compile hi.chpl.  If compiling hi.chpl does not
cause a compilation failure, start_test will then execute the resulting binary.
The concatenation of the compiler and executable output will then be compared
against the ``.good`` file.  A transcript of the test system's actions is
printed to the console and also stored in ``$CHPL_HOME/test/Logs/`` by default.


that relies on outside arguments or settings
++++++++++++++++++++++++++++++++++++++++++++

In addition to the simplest form of test shown above, the test system supports a
number of additional options for creating more complex tests.

These options are all specified using files in the same directory as the test.
Some files apply to a directory as a whole while others will apply to a single
test by sharing the same base filename.  Those files which impact the entire
directory are named in upper case, e.g. ``COMPOPTS``, or ``PERFNUMTRIALS``.
They can be overriden or augumented with test-specific settings using the same
name but in lower case, e.g. ``foo.compopts``.

Compile-time arguments
~~~~~~~~~~~~~~~~~~~~~~

To specify arguments to the compiler, provide a ``COMPOPTS`` or ``.compopts``
file for the test.  All options for a single compilation should be on the same
line - specifying multiple lines will result in multiple compilations of the
test file.

For instance, to specify that the program should be compiled statically, this
file would provided:

``hi.compopts``

  .. code-block::

     --static

To specify that the program should be compiled once statically and once
dynamically, the file would look like this:

``hi.compopts``

  .. code-block::

     --static
     --dynamic

Execution-time arguments
~~~~~~~~~~~~~~~~~~~~~~~~

Specification of arguments for execution time is performed similarly, using
a ``EXECOPTS`` or ``.execopts`` file.  Should both an ``.execopts`` and a
``.compopts`` file be provided for a test, their options will be used in
combination.  For example, a test specified like this:

``multiple-options.chpl``

  .. code-block:: chapel

    config var x = true;

    if (x) then writeln(5); else writeln(7);

``multiple-options.compopts``

  .. code-block::

    --static
    --dynamic

``multiple-options.execopts``

  .. code-block::

    --x=true
    --x=false

will be compiled twice, and executed four times by ``start_test``:

- Compilation 1:
  
  ``chpl --static multiple-options.chpl``

  - Execution 1:
    
    ``./multiple-options --x=true``

  - Execution 2:

    ``./multiple-options --x=false``

- Compilation 2:

  ``chpl --dynamic multiple-options.chpl``
  
  - Execution 3:

    ``./multiple-options --x=true``

  - Execution 4:

    ``./multiple-options --x=false``

Environment variables
~~~~~~~~~~~~~~~~~~~~~

Environment variables can be set for a particular test or directory using a
``.execenv`` or ``EXECENV`` file.  Each environment variable must be specified
on a separate line, but all will be set for a particular run.

Here is an example ``.execenv`` file:

  .. code-block::

    CHPL_RT_NUM_THREADS_PER_LOCALE=100

and control how and where it runs
+++++++++++++++++++++++++++++++++

The testing system has a variety of files that can fine tune when a test gets
run.

If the test should only be compiled and not executed, mark it with an empty file
with the suffix ``.noexec``, e.g. ``foo.noexec``.  If the test should not be
compiled or executed on its own (for instance, if it is solely a helper file for
another test), give an empty file with the suffix ``.notest``.  A directory with
an empty ``NOTEST`` file will similarly not be run by the testing system (unless
its contents are explicitly listed in the call to ``start_test``).

Controlling timeouts
~~~~~~~~~~~~~~~~~~~~

Normally, ``start_test`` will kill a test that has taken longer than 300 seconds
to execute or has been compiling for longer than four times the execution
timeout value.

The execution timeout value can be overridden for a test by specifying the
number of seconds in a ``.timeout`` file.  It can be set either higher than the
default timeout (for tests that take an unusually long time to run) or lower
(for tests that are expected to finish very quickly).  The former is used more
frequently, but the latter is useful when diagnosing a test failure - if the
test is usually quick but occasionally hangs, a smaller timeout value can help
speed up the time to run the testing system when the failure mode does occur.

Note that if the value in this file is longer than the global timeout, any
explicit ``-num-trials`` value or ``.perfnumtrials`` file will be ignored (see
`a performance test`_ for more details on the ``-num-trials`` setting).

Different settings, different output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Test should be limited to certain settings
******************************************

Sometimes a test is only applicable to certain test environments: it might rely
on multi-locale state, or change its behavior dramatically depending on if
optimizations are used, for instance.  If a test is only intended to run in
certain settings, a ``SKIPIF`` or ``.skipif`` file should be used.

A directory-wide ``SKIPIF`` file or a test-specific ``.skipif`` file can take
two forms.  The first is a line separated list of easily computed conditions,
any one of which will cause the test not to run in that particular setting.  For
instance, the following file would only allow ``foo.chpl`` to run in a
single-locale setting:

``foo.skipif``

  .. code-block::

     CHPL_COMM != none

This is useful when the conditions required to skip a test can be easily
determined from the environment.

The second form a ``.skipif`` or ``SKIPIF`` file can take is that of a script.
This form is intended for conditions that require some computation to determine,
or when the combination of conditions is necessary (i.e. this setting **and**
this setting are required for the behavior we want to avoid).  The script can be
in any commonly supported scripting language, usually bash or python.  The
``.skipif`` or ``SKIPIF`` file must have executable permissions for this form to
work.  For instance:

``foo.skipif``

  .. code-block::

     #!/usr/bin/env python

     import os
     print(os.getenv('CHPL_TEST_PERF') == 'on' and
           os.getenv('CHPL_ATOMICS') == 'locks')

would cause the test to be skipped when performance testing is done with
CHPL_ATOMICS=locks, but not ordinary performance testing, or correctness
testing with CHPL_ATOMICS=locks

Behavior in all settings is appropriate, but varies
***************************************************

If a test is intended to work in all settings but will have slightly different
behavior in some situations, it is appropriate to add additional ``.good`` files
for those settings.  Some of these additional ``.good`` files will be used
automatically by the testing system, while others will need to be specified
explicitly in the ``.compopts`` or ``.execopts`` file for the test.

No complete list of the automatically recognized ``.good`` files exists, but
some known ones include:

- ``.comm-none.good``: used with CHPL_COMM=none (the unqualified ``.good`` file
  will then apply for CHPL_COMM != none)
- ``.no-local.good``: used with ``--no-local`` testing
- ``.lm-numa.good``: used with CHPL_LOCALE_MODEL=numa
- ``.doc.good``: used when testing ``chpldoc`` instead of ``chpl``

Requests can be made for supporting additional formats if a common format
does not appear to be covered automatically.

If only some compilations or executions of a test need a specialized ``.good``
file, a comment on the same line as the relevant options can be used.  For
instance:

``foo.execopts``

  .. code-block::

     --x=true # foo.true.good
     --x=false # foo.false.good

will compare test output to ``foo.true.good`` for the first execution and
``foo.false.good`` for the second.

Any line that is unlabeled will use the default ``.good`` for that test.
Undefined behavior will occur when both the ``.compopts`` and ``.execopts``
files specify a ``.good`` file in this way.

a performance test
------------------

[Files used to illustrate the running example here can be found at
``$CHPL_HOME/test/Samples/Performance`` in the Chapel source repository]

Identifying Performance Keys
++++++++++++++++++++++++++++
Most of the information above pertains to the creation of a
correctness test, in which the test's output is compared to a ``.good``
file.  The testing system also supports performance tests in which one
or more values from a test's output can be tracked on a nightly basis
and optionally graphed.

Performance tests are specified using a ``.perfkeys`` file, which lists strings
that the test system should look for in the output serving as prefixes for a
piece of data to track.  When crawling a directory hierarchy, only tests with
``.perfkeys`` files will be considered when testing in performance mode.  For
example, if a test named ``foo.chpl`` generates output in the following format:

  .. code-block:: text

    Time: 194.3 seconds
    Memory: 24GB
    Validation: SUCCESS

one could track the two numeric values using a ``.perfkeys`` file as
follows:


``foo.perfkeys``

  .. code-block:: text

    Time:
    Memory:

As the test system runs, it will look for the specified performance
keys in the test output and store the string following the key as part
of the performance test output (described below).  Note that one could
also track the Validation string in this way, though there are better
ways to track success/failure conditions, described in the next
section.


Validating Performance Test Output
++++++++++++++++++++++++++++++++++

In addition to identifying key-value pairs to track, performance
testing can also do some simple validation of test output using
regular expression-based matching.  A line starting with
``verify:[<line#>:]`` (or ``reject:[<line#>:]``) followed by a regular
expression will ensure that the test output contains (does not
contain) the given regular expression, and count any surprises as
failures in the testing results.  The optional line# constrains what
line number the output must appear on, where a negative number
indicates that the counting should start at the end of the file.

For example, adding a third line to the ``.perfkeys`` file, we can also
verify that the last line of output contains the string "SUCCESS":

``foo.perfkeys``

  .. code-block:: text

    Time:
    Memory:
    verify:-1: SUCCESS

Accumulating Performance Data in .dat files
+++++++++++++++++++++++++++++++++++++++++++

The values collected during performance testing are stored as a
tab-delimited ``.dat`` file in the directory specified by
``$CHPL_TEST_PERF_DIR`` (if undefined, the test system defaults to
``$CHPL_HOME/test/perfdat/<machineName>``).  The base name for the ``.dat``
file is taken from the ``.perfkeys`` file.  For example, the output for
the test above would be stored in a file named ``foo.dat``.  Each time the
test is run in performance mode, a new line of data is added to the
``.dat`` file, corresponding to that run.

Note that in practice, most tests are written to be run in both a
correctness and a performance mode, using a ``bool config const`` to skip
the printing of nondeterministic data such as the Time (and possibly
Memory) values above.  We tend to make tests run in performance mode
by default and use a ``foo.execopts`` file to make the correctness testing
flip this switch (since end users will typically want the performance
data on and there's nothing worse than firing off a long run only to
find you didn't turn on the performance metrics).

Other Performance Testing Options
+++++++++++++++++++++++++++++++++

Like correctness testing, performance testing supports the ability to
specify different compiler and execution-time options, etc.  This is
done using files, as in correctness testing, where the filenames tend
to start with ``PERF*`` or ``.perf*``.  For example, ``foo.perfcompopts`` would
specify compiler options that should be used when compiling the test
for performance mode while ``foo.perfexecopts`` specifies execution-time
options for performance testing.

Comparing Multiple Versions
+++++++++++++++++++++++++++

Most performance tests are most interesting when comparing multiple
things to one another -- for example, multiple implementations of
an algorithm, a test compiled in various configurations, a Chapel vs.
C version, etc.  The approach typically taken here is to have each
configuration write output to its own ``.dat`` file and then to graph
columns from various ``.dat`` files against one another.

To compare multiple distinct Chapel tests, the approach is easy;
simply make each one a performance test with a distinct name.  (In
fact, Chapel performance tests must have unique names across the
entire testing system because all ``.dat`` files are placed into a single
directory at the end; the system itself checks for conflicts and
complains if it finds any).

To compare a C version of a test to a Chapel version, the C version of
the test must end with the suffix ``.test.c``.  Since ``.dat`` files must have
unique names, the base name for the C test should vary from the Chapel
equivalent.  For example, I might name the C version of the ``foo.chpl``
performance test ``foo-c.test.c``.  Like any other test, the C test needs
a ``.good`` file for correctness testing and a ``.perfkeys`` file for
performance testing.

To compare a single Chapel test compiled or run in multiple
configurations, the approach taken is to use multi-line versions of
the ``.perfcompopts`` OR ``.perfexecopts`` files, where each line represents a
different configuration that should be tested.  Each option line
should be concluded with a ``#`` comment delimiter, after which a
``.perfkeys`` file should be named.  For example, to compare two
problem sizes, one might use:

``bar.perfexecopts``

  .. code-block:: text

      --n=100    # bar-100.perfkeys
      --n=10000  # bar-10000.perfkeys


This would cause ``bar.chpl`` to be compiled once and executed twice, one
with ``--n=100`` and the second time with ``--n=10000``.  The first execution
would use ``bar-100.perfkeys`` for its performance keys and write its
output to ``bar-100.dat`` while the second would use ``bar-10000.perfkeys``
and write its output to ``bar-10000.dat``.

Creating a graph comparing multiple variations
++++++++++++++++++++++++++++++++++++++++++++++
Once you are creating multiple ``.dat`` files containing data you would
like to graph, you'll create a ``.graph`` file indicating which data from
which ``.dat`` files should be graphed.  For example, to compare the
timing data from the ``foo.chpl`` and ``foo-c.c`` tests described above, one
might use the following ``foo.graph`` file (note that the graph file's
base name need not have any relation to the tests it is graphing since
they are typically pulling from multiple ``.dat`` files; making the
filename useful to human readers is the main consideration).

``foo.graph``

  .. code-block:: text

    perfkeys: Time:, Time:
    files: foo.dat, foo-c.dat
    graphkeys: Chapel version, C version
    ylabel: Time (seconds)
    graphtitle: Sample Performance Test (Bogus)


Briefly, the following three entries need to have the same arity,
corresponding to the lines in the graph:

* ``perfkeys:`` is a comma-separated list of perfkeys to graph from...
* ``files:`` ...the comma-separated list of .dat files, respectively
* ``graphkeys:`` this is a comma-separated list of strings to use in the
  graph's legend.

The following two entries are singletons:

* ``ylabel:`` a label for the graph's y-axis (the x-axis will be time
  by default)
* ``graphtitle:`` a title for the graph as a whole


Finally, add the ``.graph`` file to ``$CHPL_HOME/test/GRAPHFILES``.  This file
is separated into a number of suites (indicated by comments) followed
by graphs that should appear in those suites (a graph may appear in
multiple suites).  This file determines how graphs are organized on
the Chapel performance graphing webpages (currently hosted at
``http://chapel.sourceforge.net/perf/``).

Once the ``.graph`` file exists and is listed in ``GRAPHFILES``, running
``start_test -performance`` will cause the test system to not only create
the ``.dat`` files, but also to create a graph as described in the .graph
file.  To view the graph, point your browser to
``$CHPL_TEST_PERF_DIR/<machinename>/html/index.html``.  Then select the
suite(s) in which your graph appears, and you should see data for it.
(Note that for a new graph with only one day of data, it can be hard
to see the singleton points at first).

Test Your Test Before Submitting
++++++++++++++++++++++++++++++++

Before submitting your test for review, be sure that it works under
both ``start_test`` and ``start_test -performance`` modes when running
within the directory (or directories) in question.  Nothing is more
embarrassing than committing a test that doesn't work on day one.

Once the test(s), ``.graph`` files, and ``GRAPHFILES`` are committed to the
Chapel repository, they will start showing up on the Chapel public
pages as well.


   - `a test that tracks a failure`_

     - `that requires Chapel developer effort to fix`_
     - `that should fix itself when a dependency updates`_

Invoking start_test for
=======================

correctness testing
-------------------
The `simple example <an ordinary correctness test>`_ demonstrates invoking
``start_test`` on a single explicitly-named file.  More generally,
``start_test`` takes a list of test and directory names on the command line and
will run all tests explicitly named or contained within the directories (or
their subdirectories).  For example:

  ``start_test foo.chpl bar/baz.chpl typeTests/ OOPTests/``

will test the two explicitly-named tests (``foo.chpl`` and ``baz.chpl`` stored
in the ``bar/`` directory).  It will also recursively search for any tests
stored in the ``typeTests/`` and ``OOPTests/`` subdirectories.

If invoked without any arguments, ``start_test`` will start in the current
directory and recursively look for tests in subdirectories.

performance testing
-------------------
To run performance testing, add the ``--performance`` flag to ``start_test``
along with the traditional options.  So for example, to run this
single test in performance mode, one could use:

  ``start_test --performance foo.chpl``

When crawling a directory hierarchy, only tests with ``.perfkeys`` files
will be considered when testing in performance mode.

All performance tests are compiled with ``--fast`` by default and ``--static``
when it's not problematic for the target configuration.
