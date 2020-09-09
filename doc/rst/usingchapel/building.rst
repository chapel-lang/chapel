.. _readme-building:

===============
Building Chapel
===============

To build the Chapel compiler, set up your environment as described in
:ref:`chapelhome-quickstart` (or :ref:`readme-chplenv` for more
settings), cd to ``$CHPL_HOME``, and run GNU make.

On many systems, GNU make is available simply as ``make``. On others, it is
called ``gmake``. So, first check if ``make`` refers to GNU make:

.. code-block:: bash

   # If the first line includes "GNU Make", you have GNU Make.
   make -v

Further instructions will assume ``make`` refers to GNU Make. If that's not
the case, you'll need to replace ``make`` with ``gmake``.

Now build the Chapel compiler:

.. code-block:: bash

   cd $CHPL_HOME
   make

Parallel builds such as ``make -j 6`` are also supported.

If everything works as intended, you ought to see:

1. each of the compiler source subdirectories being compiled

#. the compiler binary getting linked and stored as:

     ``$CHPL_HOME/bin/$CHPL_HOST_PLATFORM/chpl``

#. the runtime support libraries being compiled, archived, and stored
   in a configuration-specific subdirectory under:

     ``$CHPL_HOME/lib/$CHPL_TARGET_PLATFORM.$CHPL_TARGET_COMPILER.../``

If you get an error or failure during the make process, first
double-check that you have the required prerequisites (see
:ref:`readme-prereqs`). If you do, please submit a bug report for the failure
and any workaround that you come up with, through :ref:`readme-bugs`

Note that each make command only builds the compiler and runtime for
the current set of ``CHPL_`` environment variables defined by (and
inferred for) your environment.  Thus, while the directory structure
above supports the ability to have multiple versions of the compiler
and runtime built simultaneously, only one version will be created for
each make command.  To support additional host/target platforms,
host/target compilers, or threading/communication layers, you will
need to reset your environment variables and re-make.

After a successful build, you should be able to run the compiler and
display its help message using:

.. code-block:: sh

  chpl --help

In which case, you will be ready to move on to compiling with the
Chapel compiler (see :ref:`readme-compiling`).  The rest of this
file gives more information about Chapel's Makefiles for advanced
users or developers of Chapel.


----------------
Platform Support
----------------

Currently supported platforms include 32- and 64-bit Linux, Mac OS X,
Cygwin (Windows), SunOS, a variety of current Cray platforms, and a
few systems by other vendors.  Most UNIX-based environments ought to
support Chapel (subject to the assumptions in :ref:`readme-prereqs`), but may
not be supported "out-of-the-box" by our current Makefile structure.
See the section below on platform-specific settings for more
information on adding support for additional UNIX-compatible
environments.

Note that a single Chapel installation can simultaneously support
Chapel for multiple platforms and compiler options because all
platform-specific binary files and executables are stored in
subdirectories named by ``CHPL_`` environment variables.


----------------
Makefile Targets
----------------

The Chapel sources are structured so that a GNU-compatible make
utility can be used in any source directory to build the sources
contained in that directory and its subdirectories.  All of these
Makefiles support the following targets:

  +-----------+------------------------------------------------------+
  | Target    | Action                                               |
  +===========+======================================================+
  | (nothing) | Build the appropriate output files e.g. objects,     |
  | default   | libraries, executables                               |
  | all       |                                                      |
  +-----------+------------------------------------------------------+
  | clean     | Remove the intermediate files for this configuration |
  +-----------+------------------------------------------------------+
  | cleanall  | Remove the intermediate files for all configurations |
  +-----------+------------------------------------------------------+
  | clobber   | Remove everything created by the Makefiles           |
  |           | Note: make clobber will remove chplconfig            |
  +-----------+------------------------------------------------------+
  | install   | Install chapel to a previously configured location   |
  +-----------+------------------------------------------------------+

Each target processes all subdirectories then the current directory.


----------------
Makefile Options
----------------

The Chapel makefiles have a few options that enable or disable optimization,
debugging support, profiling, and backend C compiler warnings. The variables
are described below. Set the value to 1 to enable the feature.

  ========  =======================================================
  Option    Effect
  ========  =======================================================
  DEBUG     Generate debug information (e.g. add -g to C compiler).
  OPTIMIZE  Enable optimizations (e.g. add -O3 to C compiler).
  PROFILE   Enable profiling support (e.g. add -pg to C compiler).
  WARNINGS  Promote backend C compiler warnings to errors.
  ASSERTS   Enables correctness assertions in the compiler and runtime.
  ========  =======================================================


.. _platform-specific-settings:

--------------------------
Platform-specific Settings
--------------------------

The structure of Chapel's Makefiles is designed to factor any
compiler-specific settings in
``$CHPL_HOME/make/compiler/Makefile.<compiler>`` where ``<compiler>`` refers
to ``$CHPL_HOST_COMPILER`` for the compiler sources and
``$CHPL_TARGET_COMPILER`` for the runtime sources and generated code.
Refer to :ref:`readme-chplenv` for more information about these variables and
their default settings.

In addition, any architecture-specific settings are defined in
``$CHPL_HOME/make/platform/Makefile.<platform>``, where ``<platform>`` refers
to ``$CHPL_HOST_PLATFORM`` for the compiler sources and
``$CHPL_TARGET_PLATFORM`` for the runtime sources and generated code.
Again, :ref:`readme-chplenv` details these variables and their default
settings.

If you try making the compiler and runtime for an unknown platform, it
will assume that you want to use gcc/g++ to compile the code and that
you require no platform-specific settings.  You can add support for a
new build environment by creating ``Makefile.<compiler>`` and/or
``Makefile.<platform>`` files and setting your environment variables to
refer to those files.  If you do develop new build environment support
that you would like to contribute back to the community, we encourage
you to submit a pull request to the `Chapel GitHub repository`_.

.. _readme-installing:

-----------------
Installing Chapel
-----------------

Chapel can be built and installed as follows:

.. code-block:: bash

  ./configure # adding appropriate options
  make
  make install # possibly with elevated privilege

See ``./configure --help`` for more information on the options available.

.. note::

 ``./configure`` will save the current configuration into a
 ``chplconfig`` file and can set the installation path that will be
 compiled in to the ``chpl`` binary.


.. _Chapel GitHub repository: https://github.com/chapel-lang/chapel
