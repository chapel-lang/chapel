.. _readme-executing:

=========================
Executing Chapel Programs
=========================

Once you have compiled a Chapel program using the chpl compiler, you
can execute it from the command-line like any other program.  This
document discusses command line options, environment variables, and
other topics having to do with running programs written in Chapel.

.. contents::


------------
Getting Help
------------

Using the ``-h`` or ``--help`` flags will print out help for the
executable.  For example:

  .. code-block:: sh

    ./a.out --help

This flag lists all of the standard flags that can be used with a
Chapel program, as well as a list of the configuration variables
defined by the program and their types.  If the configuration variable
has been set on the command line, its value is also shown.


-------------------------------
Setting Configuration Variables
-------------------------------

Configuration constants and variables defined in a Chapel program can
have their default values overridden on the command line using the ``-s``
or ``--`` flags.  Either flag takes the name of the configuration variable
followed by an equals character (``=``) and the value to assign to it.
This value must be a legal Chapel literal for the type of the variable.
(Exception: for a string literal, the surrounding quotes are implicit.)
In our current implementation, no extra spaces may appear between
these elements.  Thus, the general form is:

  .. code-block:: sh

    ./a.out --<cfgVar>=<val>

or:

  .. code-block:: sh

    ./a.out -s<cfgVar>=<val>

As an example, compile the hello2-module.chpl example which prints a
user-definable message:

  .. code-block:: sh

    chpl -o hello2-module $CHPL_HOME/examples/hello2-module.chpl

This program defines a configuration constant, ``message``, indicating
the string to print to the console, set to ``"Hello, world!"`` by
default.  To override the default, you can use:

  .. code-block:: sh

    ./hello2-module --message='Hello, Chapel users!'

or:

  .. code-block:: sh

    ./hello2-module -smessage='Hello, Chapel users!'

The default value of a configuration constant or variable can also be
overridden at compilation time with a ``-s`` option. (The surrounding
quotes must be provided for a string literal.) For example:

  .. code-block:: sh

    cd $CHPL_HOME/examples
    chpl -smessage='"Hello from the Chapel compiler"' hello2-module.chpl

or:

  .. code-block:: sh

    chpl -s message="'Hello from the Chapel compiler'" hello2-module.chpl

The compiler-established default can still be overridden when
executing the program, as shown above.

Chapel programs can also accept C-like command line arguments to their
``main()`` procedure in addition to the aforementioned configuration
variables. See :ref:`readme-main` for more
information.


-----------------------------
Setting the Number of Locales
-----------------------------

For multi-locale Chapel executions, the number of locales on which to
execute a program is specified on the executable's command-line.  This
can be set either using the -nl flag, or by assigning to the built-in
numLocales configuration constant using the normal mechanisms.  So, to
execute on four locales, one could use:

  .. code-block:: sh

    ./a.out -nl 4

or:

  .. code-block:: sh

    ./a.out --numLocales=4

or:

  .. code-block:: sh

    ./a.out -snumLocales=4

For users running with ``CHPL_COMM=none`` (the default), only one
locale can be used.  See :ref:`readme-multilocale` for more
information about executing on multiple locales.

Multi-locale programs often use a launcher executable to do some initial
command-line checking before spawning the real program, which is then
stored in a second binary named ``<original_binary_name>_real``.  See
:ref:`readme-launcher` for more information about the launcher executable.


--------------------------------------
Controlling Degree of Data Parallelism
--------------------------------------

Data parallel operations over ranges, default domains, and default
arrays permit the number of tasks used to implement the data
parallelism to be specified using the following built-in configuration
constants:

  ``dataParTasksPerLocale``
    Number of Chapel tasks to use to execute forall loops (default:
    number of physical CPUs on the node, which may be reduced by
    ``CHPL_RT_NUM_THREADS_PER_LOCALE``.  When ``CHPL_TASKS=qthreads``,
    ``CHPL_RT_NUM_THREADS_PER_LOCALE`` can also increase this, up to the
    number of logical CPUs).

  ``dataParIgnoreRunningTasks``
    If ``true``, always use ``dataParTasksPerLocale`` tasks to execute
    forall loops.  If ``false``, reduce the number of tasks used by the
    number of tasks already running (default: ``false``).

  ``dataParMinGranularity``
    The number of tasks used to execute forall loops should be reduced
    such that the number of iterations per task is never less than the
    specified value (default: ``1``).

Most Chapel standard distributions also use identically named
constructor arguments to control the degree of data parallelism within
each locale when iterating over its domains and arrays.  The default
values for these arguments are set to the corresponding global
configuration constants.


-------------------------------------------------
Environment Variable Control over Chapel Behavior
-------------------------------------------------

Chapel uses environment variables to control the number of threads used
at execution time and the call stack size, among other things.  In many
cases third-party packages used by Chapel define their own environment
variables to provide the same or similar control.  When this is the
case, the Chapel environment variable has precedence over the
third-party package environment variable, which in turn has precedence
over the Chapel default.

As an example, with ``CHPL_TASKS=qthreads``, Chapel and Qthreads both
have environment variables that can be used to set the task stack size.
In this case the Chapel ``CHPL_RT_CALL_STACK_SIZE`` environment variable
will override the Qthreads ``QT_STACK_SIZE`` environment variable if
both are set.  However, if only ``QT_STACK_SIZE`` is set it will
override the Chapel default call stack size.

The Chapel environment variables that control execution time behavior
are as follows:

  ``CHPL_RT_CALL_STACK_SIZE``
    size of the call stack for a task

  ``CHPL_RT_MAX_HEAP_SIZE``
    per-locale size of the heap used for dynamic allocation in
    multilocale programs

  ``CHPL_RT_NUM_THREADS_PER_LOCALE``
    number of threads used to execute tasks

There is a bit more information on ``CHPL_RT_CALL_STACK_SIZE`` and
``CHPL_RT_NUM_THREADS_PER_LOCALE`` below, and more detailed discussion
of all of these in :ref:`readme-tasks` and :ref:`readme-cray`.


-------------------------------
Controlling the Call Stack Size
-------------------------------

The main Chapel program requires space for its call stack, as does any
task created by it.  This stack space has a fixed size.  It is created
automatically when the program or task starts executing, and remains in
existence until it completes.  The default call stack size is 8 MiB on
Linux-based systems, since this is a common value for the process stack
limit on such systems.  On Cygwin systems the default call stack size is
2 MiB.

The default call stack size may not be appropriate in all cases.  For
programs in which some tasks have large stack frames or deep call trees
it may be too small, leading to stack overflow.  For programs which use
tasks only for data parallelism it may be unnecessarily large.  Stacks
that are unnecessarily large are typically only a problem for programs
in which many tasks (thus their stacks) exist at once, when using a comm
layer that has to pre-register memory.  For the particular case of using
the native runtime communication and tasking layers on Cray X* systems,
further discussion about this can be found in :ref:`readme-cray`.

The following environment variable can be used to change the task call
stack size.

  ``CHPL_RT_CALL_STACK_SIZE``
    Size of the call stack for a task.  A plain numeric value indicates
    bytes.  A suffix can be appended to indicate larger units:

     | ``k``, ``K``: KiB (2**10 bytes)
     | ``m``, ``M``: MiB (2**20 bytes)
     | ``g``, ``G``: GiB (2**30 bytes)


---------------------------------
Controlling the Number of Threads
---------------------------------

The following environment variable can be used to change the number of
system threads used by a program.

  ``CHPL_RT_NUM_THREADS_PER_LOCALE``
    Controls the number of threads used on each locale when running the
    program.

See :ref:`readme-tasks` for more information on the role of
this variable in creating threads and executing tasks for the various
tasking layers.


-----------------------------------------
Controlling the Amount of Non-User Output
-----------------------------------------

The compiler-generated executable supports *verbose* and *quiet* modes
that control the amount of Chapel-generated information printed by the
executable.

    -v, --verbose  Print more information. For example, print the
                   launcher commands used to start the program (if any)
                   and print a message from each locale when the program
                   starts executing there.
    -q, --quiet    Print less information. For example, suppress run-time
                   warnings that are printed by default.


----------------
Launcher Support
----------------

For multilocale execution (see :ref:`readme-multilocale`),
Chapel programs are executed indirectly by a launcher.  This section
covers command line options that assist launchers in doing their job.
These options are not supported for general use.  We document them here
so that their presence in, say, the verbose output produced by ``-v``
can be understood.

At present there is only one launcher support option:

  -E <envVar=val>  set the given environment variable *envVar* to *val*.
