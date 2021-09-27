.. _readme-building:

===============
Building Chapel
===============

To build the Chapel compiler, set up your environment as described in
:ref:`readme-chplenv`, cd to ``$CHPL_HOME`` (the root directory of
your unpacked Chapel release), and run GNU make.  This will build the
compiler and runtime libraries for your current environment as
indicated by running ``$CHPL_HOME/util/printchplenv --all``.

On many systems, GNU make is available simply as ``make``. On others,
it is called ``gmake``.  To check whether ``make`` refers to GNU make
on your system, run:

.. code-block:: bash

   make -v

If the first line includes "GNU Make", you have GNU Make.  The rest of
these instructions will assume ``make`` refers to GNU Make. If that's
not the case, you'll need to replace ``make`` with ``gmake``.

Now build the Chapel compiler:

.. code-block:: bash

   cd $CHPL_HOME
   make

Parallel builds such as ``make -j 6`` are also supported.

If everything works as intended, you ought to see:

1. each of the compiler source subdirectories being compiled

#. the compiler binary getting linked and stored as:

     ``$CHPL_HOME/bin/$CHPL_HOST_PLATFORM-$CHPL_HOST_ARCH/chpl``

#. the runtime support libraries being compiled, archived, and stored
   in a configuration-specific subdirectory under:

     ``$CHPL_HOME/lib/$CHPL_TARGET_PLATFORM/.../``

If you get an error or failure during the make process, double-check
that you have the required prerequisites (see
:ref:`readme-prereqs`). If you do, please submit a bug report for the
failure through :ref:`readme-bugs`.

After a successful build, you should be able to run the compiler and
display its help message using:

.. code-block:: sh

  chpl --help

In which case, you are ready to move on to compiling with the Chapel
compiler (see :ref:`readme-compiling`).  The next few sections provide
more information about using Chapel across multiple sessions,
installing Chapel, and supporting multiple Chapel configurations.
Following that are sections describing Chapel's Makefiles for advanced
users or developers of Chapel.


.. _using-chapel-in-other-sessions:

------------------------------------------------------
Using Chapel in a Different Shell / Terminal / Session
------------------------------------------------------

Once you have built Chapel, you can use it in other sessions (new
shells or terminals) as long as:

1. the ``chpl`` compiler is in your ``$PATH``

#. your ``$CHPL_`` environment variables are set or inferred to values
   that match a configuration you have already built

Typically, this is accomplished in one of the following ways:

* by manually re-sourcing the ``setchplenv.*`` file that you used
  when building Chapel.

* by editing your 'dot files' (e.g., ``.bashrc``, ``bash_profile``, or
  whatever your shell uses to start up each session) to explicitly set
  your path and ``$CHPL_`` settings as desired, or to source the
  appropriate ``setchplenv.*`` script.

* by creating a ``chplconfig`` file to store your preferred ``$CHPL_``
  settings (see :ref:`readme-chplenv.chplconfig` for further details
  on creating a chplconfig file).  Note, however, that some other
  standard technique is still required to ensure that ``chpl`` is in
  your path.

* by installing Chapel to a specific location that is in your path
  using the instructions in the next section.


.. _readme-installing:

-----------------
Installing Chapel
-----------------

Chapel can be built and installed to a specific location as follows:

.. code-block:: bash

  ./configure  # use './configure --help' for specific options
  make
  make install

Running ``./configure`` will save your current configuration of
``$CHPL_`` settings into a ``chplconfig`` file.  Running it with the
``--prefix`` or ``--chpl-home`` options permits you to specify where
and how Chapel should be installed during the ``make install`` step.
Specifically:

* ``--prefix=/dir/for/install/`` causes the Chapel compiler,
  libraries, and supporting code to be installed into the directories:

  - ``/dir/for/install/bin``
  - ``/dir/for/install/lib``
  - ``/dir/for/install/share``

  This technique is designed to install Chapel using a standard
  directory structure for the purposes of integrating it into a
  standard location that is already in your path, such as
  ``/usr/local/`` or ``~/``.  Note that elevated privileges are likely
  to be required for any system-wide installation locations.

* ``--chpl-home=/dir/for/install`` copies key files and directories
  from the Chapel source tree into ``/dir/for/install``, preserving
  Chapel's traditional directory structure.


-----------------------------------------
Switching Between Multiple Configurations
-----------------------------------------

A single installation of Chapel can simultaneously support multiple
configuration options, such as target platforms or compilers, because
all binary files and executables are stored in unique subdirectories
as determined by the ``CHPL_`` environment variables.  However, note
that each ``make`` command only builds the compiler and runtime for
the current set of ``CHPL_`` environment variables defined by—and
inferred for—your environment.  To build support for additional
configurations, you will need to modify your ``CHPL_`` environment
variables and re-make.

Once you have built multiple configurations, you can switch between
them either by setting your ``CHPL_`` environment variables to the
same values as when the configuration was built, or using the ``chpl``
compiler's command-line options that correspond to the environment
variables (see the 'Compiler Configuration Options' section of the
:ref:`man-chpl` man page for details).


----------------
Makefile Targets
----------------

The Chapel sources are structured so that a GNU-compatible ``make``
utility can be used in any source directory to build the sources
contained in that directory and its subdirectories.  All of these
Makefiles support the following targets:

  +-----------+------------------------------------------------------+
  | Target    | Action                                               |
  +===========+======================================================+
  | (nothing) | Build the appropriate targets — e.g., objects files, |
  | default   | libraries, executables                               |
  | all       |                                                      |
  +-----------+------------------------------------------------------+
  | check     | verify basic functionality of a Chapel build         |
  +-----------+------------------------------------------------------+
  | clean     | Remove the intermediate files for this configuration |
  +-----------+------------------------------------------------------+
  | cleanall  | Remove the intermediate files for all configurations |
  +-----------+------------------------------------------------------+
  | clobber   | Remove everything created by the Makefiles           |
  |           | as well as ``chplconfig``                            |
  +-----------+------------------------------------------------------+
  | install   | Install Chapel to a previously configured location   |
  +-----------+------------------------------------------------------+

Each target processes all subdirectories, then the current directory.


----------------
Makefile Options
----------------

The Chapel makefiles have a few options that enable or disable
optimization, debugging support, profiling, and back-end C compiler
warnings. The variables are described below. Set the value to 1 to
enable the feature or 0 to disable it (e.g., ``make DEBUG=1 OPTIMIZE=1
WARNINGS=0``).

  ========  ================================================================
  Option    Effect
  ========  ================================================================
  DEBUG     Generate debug information (e.g., pass ``-g`` to the C compiler)
  OPTIMIZE  Enable optimizations (e.g., pass ``-O3`` to the C compiler)
  PROFILE   Enable profiling support (e.g., pass ``-pg`` to C compiler)
  WARNINGS  Enable more warnings and treat them as errors
  ASSERTS   Enables correctness assertions in the compiler and runtime
  ========  ================================================================



