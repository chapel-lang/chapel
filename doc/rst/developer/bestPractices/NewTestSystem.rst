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
   - `a test that tracks a failure`_

     - `that requires Chapel developer effort to fix`_
     - `that should fix itself when a dependency updates`_

* `Invoking start_test for correctness testing`_


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

   - `a test that tracks a failure`_

     - `that requires Chapel developer effort to fix`_
     - `that should fix itself when a dependency updates`_

Invoking start_test for correctness testing
===========================================
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
