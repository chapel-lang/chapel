.. _readme-compiling:

=========================
Compiling Chapel Programs
=========================

The Chapel compiler converts programs expressed in Chapel source code
into the corresponding executable form. This document briefly discusses
running the Chapel compiler.

.. contents::


------------
Getting Help
------------

If you have set up your environment as described in :ref:`readme-chplenv` and
built the compiler as described in :ref:`readme-building`, you can see the
Chapel compiler's help message using:

  .. code-block:: sh

     chpl --help

This displays the compiler's command-line flags sorted by category.
For each flag, the short one-letter form is given (if one exists),
followed by the long form of the flag.  Next, the pattern for any
arguments to the flag are given.  And finally a brief description of
the flag is provided.  A flag may have a corresponding environment
variable, setting which is equivalent to passing the flag.  These
variables are listed by the ``--help-env`` option.  The flags' current
settings are listed by ``--help-settings``.

More detailed information about the compiler and its command-line
flags is also available on its :any:`man page <man>`, which can be viewed from
the command-line using:

  .. code-block:: sh

     man chpl


-----------------
Most Useful Flags
-----------------

We note some of the most useful flags here:

=====================   ======================================================
Flags                   Description
---------------------   ------------------------------------------------------
``-o <filename>``       specify the filename of the generated executable,
                        otherwise the name of the file containing the main
                        module is used (minus its ``.chpl`` extension).
``--no-checks``         turns off runtime semantic checks like bounds
                        checking and nil class instance dereferencing
``-O``                  turns on optimization of the generated C code
``--fast``              turns on ``--no-checks``, ``-O``, and enables
                        many other optimizations
``-s <name[=expr]>``    set a config declaration with the given expression
                        as its default value (config params must be set
                        to values that are known at compile time)
``-M <dir>``            add the specified directory to the module search path
``--savec <dir>``       saves the generated C code in the specified
                        directory
``-g``                  support debugging of the generated C code
``--ccflags <flags>``   specify flags that should be used when invoking
                        the back-end C compiler
``--ldflags <flags>``   specify flags that should be used when invoking
                        the back-end linker
``--print-passes``      print the compiler passes as they execute
``--print-commands``    print the system commands that the compiler
                        executes
``--print-code-size``   prints some code size statistics about the
                        number of lexical tokens per line, as well as
                        the number of code, comment, and blank lines
``--version``           print the Chapel compiler version number
``--help``              print a brief overview of the command-line
                        options
``--help-env``          lists the environment variables for each
                        command-line flag
``--help-settings``     lists the current setting of each command-line
                        flag
=====================   ======================================================


------------------------
Tab Completion for Flags
------------------------

Bash users can source the script ``$CHPL_HOME/util/chpl-completion.bash`` to
enable tab-completion for chpl options.  After sourcing the
chpl-completion.bash script tab completion can be used:

  .. code-block:: sh

     % chpl --ca<tab><tab>

Will print the options that start with "--ca".

  .. code-block:: sh

     --cache-remote --cast-checks

Adding one more letter to differentiate and pressing tab again will
auto-complete the option and add a space, ready for the next option.

  .. code-block:: sh

     % chpl --cac<tab>
     % chpl --cache-remote

------------------------------
Integrating Into Build Systems
------------------------------

Larger projects using Chapel may wish to setup a build system to manage
compilation of their Chapel code. This section provides a brief overview of a
few common build systems and how they can be used with Chapel.

Mason
^^^^^

:ref:`Mason <readme-mason>` is the Chapel package manager and build system. See
the :ref:`Mason Guide <index-mason-guide>` for a walkthrough of how to use
Mason to manage your Chapel projects.

Make
^^^^

If you wish to use Make to compile your Chapel code, you can specify a recipe
that contains all of the Chapel files and flags in a single invocation. Chapel
currently does not support separate compilation, so all Chapel files must be
compiled together.

For example, a simple ``Makefile`` that compiles a Chapel project might look
like this. This can be extended with other flags or dependencies as needed:

.. code-block:: Makefile

   CHPL := chpl
   DEBUG ?= 0
   TARGET := myProgram

   .PHONY: all
   all: $(TARGET)

   ifeq ($(DEBUG),1)
   CHPL_FLAGS += -g
   else
   CHPL_FLAGS += --fast
   endif

   SOURCES := $(wildcard *.chpl)

   $(TARGET): $(SOURCES)
   	$(CHPL) $(CHPL_FLAGS) $< -o $@

   .PHONY: clean
   clean:
   	rm -f $(TARGET)

This can be used by running ``make`` in the directory containing the Makefile.
Running ``make DEBUG=1`` will compile the program with debugging enabled and
``make clean`` will remove the compiled program.

CMake
^^^^^

Although CMake does not yet support Chapel programs natively, the
Chapel source tree contains a directory,
`util/cmake <https://github.com/chapel-lang/chapel/tree/main/util/cmake>`_,
which can be added to a CMake project to support building Chapel programs.

To enable CMake support for your Chapel project, take the following steps:

#. Copy the ``util/cmake`` directory from the Chapel source tree to your
   project. You may want to copy the files to a subdirectory like ``cmake``, or
   you can just copy them to the root of your project.
#. Add ``find_package(chpl REQUIRED HINTS .)`` to your ``CMakeLists.txt`` file.

For example, a simple ``CMakeLists.txt`` file that compiles a Chapel project
might look like this:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   find_package(chpl REQUIRED HINTS .)

   project(myProgram LANGUAGES CHPL)

   add_executable(myProgram main.chpl myModule.chpl)

Based on the value of ``CMAKE_BUILD_TYPE`` additional flags will be
added, for example ``--fast`` for ``CMAKE_BUILD_TYPE=release``.

This can then be used like any other CMake project. For example, the following
will build a release version of the program:

.. code-block:: sh

   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=release
   cmake --build .
