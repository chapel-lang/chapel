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
foo.chpl            test program to compile and run
foo.good            expected output of test program
foo.compopts        line separated compiler flag configurations
COMPOPTS            directory-wide compiler flags
foo.execopts        line separated runtime flag configurations
EXECOPTS            directory-wide runtime flags
foo.prediff         script that is run on the test output, before taking the
                    diff between the output and .good file
..
-------------------------------------------------------------------------------
**performance**
-------------------------------------------------------------------------------
foo.perfcompopts    compiler flags, overrides .compopts for --performance
foo.perfexecopts    runtime flags, overrides .execopts for --performance
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
                    *keyword*, *short description*, *long description*
foo.bad             output generated on a failing test, to track if a known
                    failing future begins failing a different way
..
=================   ===========================================================


.. _correctness:

Correctness Tests
=================

.. TODO: Recursive behavior of COMPOPTS/EXECOPTS?
.. TODO: Specifying .good files in compopts/execopts
.. TODO: Which files can be treated as an executable script?
.. TODO: New .pre* files


Though trivial, this test is available at ``$CHPL_HOME/test/Samples/Correctness``
in the Chapel source repository

Creating a Simple Test
----------------------
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

This will cause the compiler to compile hi.chpl then execute the
resulting binary.  The concatenation of the compiler and executable
output will then be compared against the ``.good`` file.  A transcript of
the test system's actions is printed to the console and also stored in
``$CHPL_HOME/test/Logs/`` by default.


Invoking start_test for correctness testing
-------------------------------------------
The simple example above demonstrates invoking ``start_test`` on a single
explicitly-named file.  More generally, ``start_test`` takes a list of
test and directory names on the command line and will run all tests
explicitly named or contained within the directories (or their
subdirectories).  For example:

  ``start_test foo.chpl bar/baz.chpl typeTests/ OOPTests/``

will test the two explicitly-named tests (``foo.chpl`` and ``baz.chpl`` stored
in the ``bar/`` directory).  It will also recursively search for any tests
stored in the ``typeTests/`` and ``OOPTests/`` subdirectories.

If invoked without any arguments, ``start_test`` will start in the current
directory and recursively look for tests in subdirectories.


Additional correctness testing options
--------------------------------------
In addition to the simplest form of test shown above, the test system
supports a number of additional options for creating more complex
tests.  For example, capabilities exist to specify compilation and
execution command-line flags for a test, to post-process the test
output (e.g., to remove nondeterminism), to skip over the test under
certain conditions, etc.

These options are all specified using files in the same directory as
the test.  Some files apply to a directory as a whole while others
will apply to a single test by sharing the same base filename.  As an
example, the file ``COMPOPTS`` specifies compiler options that should be
used for all tests in the given directory while ``hi.compopts`` would
specify compiler options that should only be used for the ``hi.chpl``
test.

For more information on these capabilities, review the comments at the
top of ``start_test`` which is currently the most complete documentation
on the testing system.  (NOTE: Over time, we want to migrate that
documentation over to this text file).


.. _performance:

Performance Tests
=================

.. TODO: Restrictions in perfcompopts / perfexecopts w.r.t multiple configs
.. TODO: Specifying .dat files in perf*opts
.. TODO: All options for .graph files


[Files used to illustrate the running example here can be found at
``$CHPL_HOME/test/Samples/Performance`` in the Chapel source repository]

Identifying Performance Keys
----------------------------
Most of the information above pertains to the creation of a
correctness test, in which the test's output is compared to a ``.good``
file.  The testing system also supports performance tests in which one
or more values from a test's output can be tracked on a nightly basis
and optionally graphed.

Performance tests are specified using a ``.perfkeys`` file, which lists
strings that the test system should look for in the output serving as
prefixes for a piece of data to track.  For example, if a test named
``foo.chpl`` generates output in the following format:

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
----------------------------------
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


Invoking start_test for performance testing
-------------------------------------------
To run performance testing, add the ``--performance`` flag to ``start_test``
along with the traditional options.  So for example, to run this
single test in performance mode, one could use:

  ``start_test --performance foo.chpl``

When crawling a directory hierarchy, only tests with ``.perfkeys`` files
will be considered when testing in performance mode.

All performance tests are compiled with ``--fast`` by default and ``--static``
when it's not problematic for the target configuration.


Accumulating Performance Data in .dat files
-------------------------------------------
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
---------------------------------
Like correctness testing, performance testing supports the ability to
specify different compiler and execution-time options, etc.  This is
done using files, as in correctness testing, where the filenames tend
to start with ``PERF*`` or ``.perf*``.  For example, ``foo.perfcompopts`` would
specify compiler options that should be used when compiling the test
for performance mode while ``foo.perfexecopts`` specifies execution-time
options for performance testing.


Comparing Multiple Versions
---------------------------
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
----------------------------------------------
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
--------------------------------
Before submitting your test for review, be sure that it works under
both ``start_test`` and ``start_test -performance`` modes when running
within the directory (or directories) in question.  Nothing is more
embarrassing than committing a test that doesn't work on day one.

Once the test(s), ``.graph`` files, and ``GRAPHFILES`` are committed to the
Chapel repository, they will start showing up on the Chapel public
pages as well.



.. _futures:

Futures: A mechanism for tracking bugs, feature requests, etc.
==============================================================

The testing system also serves as our current system for tracking
code-driven bugs and open issues.  In particular, any test can be
marked as being a "future" test indicating that it doesn't work today
but should in the future (or else be removed from the testing system).

To mark a test as a future, you add a ``.future`` file sharing the same
base name as the test.  For example, adding a hi.future file would
make the simple correctness test at the start of this document into a
future test.  Marking a test as a ``.future`` causes it to be tested every
night, but not to be counted against the compiler's success/failure
statistics.  Like normal correctness tests, future tests still need to
have a ``.good`` file indicating their expected output so that if/when
they start working, developers will become aware of it.

In addition to a ``.good`` file, it is usually considered good practice to
add a ``.bad`` file which captures the current output of the failing test.
This is useful so that, if the test starts to fail for a different
reason (e.g., the language syntax changes), the original cause of the
test is not masked by the new failure.  Tests whose current/``.bad``
output varies based on the compiler version number, line numbers of
standard modules and such are fragile since these things change
frequently; in such cases, either a ``.prediff`` should be used to filter
the output before comparing to ``.bad``, or the ``.bad`` should be omitted.
Ultimately, our intention is to support a library of common recipes
for ``.bad`` files, but this has not been implemented yet.

The format of the ``.future`` file itself is minimally structured.  The
first line should contain the type of future (see list below) followed
by a brief (one 80-column line) description of the future.  The rest
of the file is free-form and can be used over the future's lifetime to
describe in what way the test isn't working or should be working,
implementation notes, philosophical arguments, etc.  The one-line
summaries of all outstanding futures can be viewed by running
``$CHPL_HOME/util/devel/test/list_futures``.

The current categories of future are:

* bug: this test exhibits a bug in the implementation

* error message: this test correctly generates an error message, but
    the error message needs clarification/improvement

* feature request: a way of filing a request for a particular feature
  in Chapel

* memory: indicates a test that exhibits a problem with memory usage
  (such as a memory leak)

* multilocale: shows a problem that only relates to multi-locale
    executions

* performance: indicates a performance issue that needs to be addressed

* semantic:  this test raises a question about Chapel's semantics
    that we ultimately need to address

* unimplemented feature: this test uses features that are specified, but
    which have not yet been implemented.

**JIRA and futures**

Currently, it is considered good practice to include a future for an issue
tracked on the `JIRA page`_. You can optionally include a back-reference to
this JIRA issue from the future, to remind developers to mark the issue
complete when the future is resolved.

.. _`JIRA page`: https://chapel.atlassian.net/projects/CHAPEL/issues

.. _extensions:

Planned Changes of Testing System
=================================

**Migrate to yaml-based system**

It has been proposed to move away from the current system of 1 file per type of
configurations, and opt for a yaml-based system. This would require a
significant overhaul of the testing infrastructure, and consequently would take
a lot of careful planning and development. For the time being, this idea
remains backlogged on our testing wish list.

**Support performance tracking of third-party codes**

There is a desire to do more comprehensive comparisons of Chapel to other
languages, particularly in benchmark suites. This system would likely involve
scripts that would mirror a internal copy of, build, run, and gather timings
for reference versions. This data could be shown on the performance tracking
page.

**Documentation Improvements**

There are several aspects of the testing system undocumented.
See the ``..  TODO`` lines within the raw text of this file to see a few.
