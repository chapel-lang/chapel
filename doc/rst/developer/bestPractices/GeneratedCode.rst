.. _best-practices-generated-code:

=================================
All About Compiler-Generated Code
=================================

Where is it? How to compile it?
-------------------------------

By default: in a temporary directory, deleted when the compiler exits (only the executable remains).

``--savec DIRECTORY``:

  makes it go to DIRECTORY and stay there.


To recompile: 
    
  ``make -f DIRECTORY/Makefile``

``--print-commands``: 

  shows commands issued by the compiler


.. _more-munging-info:

Why are my identifiers renamed in the generated code?
-----------------------------------------------------

* Note that all user identifiers are munged by default in Chapel's
  generated code to prevent inadvertent conflicts with identifiers
  from other C libraries and headers against which Chapel is linked
  (see the ``chpl`` man page for ``--[no-]munge-user-idents`` for more
  information).  This can have the downside of making the generated
  code not correspond with the Chapel source in a way that can make
  reading the code, as well as debugging and profiling it, slightly
  more complicated.  Compiling with the ``--no-munge-user-idents`` flag
  can reduce the degree to which this is done (and for most programs,
  should still work fine).

* In some cases, Chapel does additional munging in order to turn legal
  Chapel into legal C (e.g., Chapel supports overloads but C doesn't).
  This munging cannot be disabled by a flag and is typically necessary
  (though Chapel is sometimes overly conservative in deciding to
  munge).  Generally, the more C-like your code is, the less this
  should happen.


How to benchmark/time it?
-------------------------

* in Chapel: you want to time only the important code,
  excluding the startup time. See the `Time Module`_. 

 ::

  use Time;
  var mytimer: stopwatch;
  
  mytimer.restart();

  ... measured code goes here ...
  mytimer.stop();

  writeln("time taken: ", mytimer.elapsed(), " seconds");

.. _Time Module: https://chapel-lang.org/docs/modules/standard/Time.html

* build your runtime optimized:

  ::

      cd $CHPL_HOME/runtime
      make clean
      make DEBUG=0 OPTIMIZE=1

  Note: currently you can have only one compilation of the runtime.Manipulate ``$CHPL_HOME/{runtime,lib}`` yourself if you want to have
  one copy for timing, another for debugging, etc.


* when compiling, use:

      ``--fast``

  This option (a) removes safety checks (e.g. for null pointers,
  array index bounds, etc.), and (b) compiles the generated code
  with optimization.

* in C code: mimick the Chapel runtime, for apples-to-apples, e.g:

 .. code-block:: c
 
  // from chpltypes.h

  typedef double _real64;

  #define chpl_seconds_timer(time) ((_real64)((time).tv_sec))

  #define chpl_microseconds_timer(time) ((_real64)((time).tv_usec))

  // current timer, in fractional seconds
  double gettimer(struct timeval *timer) {
     int dummy = gettimeofday(timer, NULL);  // ignore the return code
     return chpl_seconds_timer(*timer) + 1.0e-6 * chpl_microseconds_timer(*timer);
  }

  struct timeval mytimer;
  double start = gettimer(&mytimer);

  ... measured code goes here ...
  double elapsedSeconds = gettimer(&mytimer) - start;


Debugging the generated code
----------------------------

Using gdb and other debugging features are also discussed in:

      ``$CHPL_HOME/doc/rst/usingchapel/debugging.rst``

* If you want to debug the runtime library as well, build your runtime so:

  ::

      cd $CHPL_HOME/runtime
      make clean
      make DEBUG=1 OPTIMIZE=0

  Note: currently you can have only one compilation of the runtime (see above).


* When compiling, use:

      ``-g --savec DIRECTORY``


* By default gdb will step through/refer to the Chapel source code.

  To make gdb find it, put the following in your ``~/.gdbinit``:

::

 define sdirs
  directory $arg0/modules/internal
  directory $arg0/modules/standard
  directory $arg0/modules/dists
  directory $arg0/modules/layouts
 end

then call ``sdirs`` from the gdb prompt (do not use `~` or `$CHPL_HOME`), e.g.:

::
 
 (gdb) sdirs /users/vass/chapel
 (gdb) 


* To have gdb refer to the actual generated C code, compile with:

      ``-g --savec DIRECTORY2 --c-line-numbers``


* GDB's TUI mode is discussed in ``CompilerDebugging.rst`` and, e.g., here:

      http://davis.lbl.gov/Manuals/GDB/gdb_21.html


* To see the IDs of the AST nodes in the generated code
  (see ``CompilerIRTricks.txt``):

      ``--gen-ids``


Profiling the generated code
----------------------------

Some Chapel programmers have had success profiling Chapel's generated
code using ``gprof`` using the instructions given below.  Others have
had successes with the Linux-based ``perf`` command, HPCToolkit, or
other performance analysis tools (not covered here).

To use ``gprof``:

* Note that, at present, this is only supported when using the C
  back-end, not the LLVM back-end.  As a result,
  ``CHPL_TARGET_COMPILER`` must be set or inferred to something other
  than ``llvm``.

* For best results, be sure your runtime is compiled with
  ``OPTIMIZE=1`` to reduce the time spent in runtime code.  You may
  also want to build it with ``PROFILE=1`` in order to have it
  included in your profiling report.

* When compiling, use:

      ``--ccflags -pg --ldflags -pg --fast --savec DIRECTORY``

  This produces a `gprof-enabled` executable.

  The use of ``--fast`` and ``--savec`` are not strictly necessary,
  but will improve execution time and give you access to the generated
  C code (in the named directory), respectively.


Miscellanea
-----------

`grepcomp`

`greprt`

`grepmod`

`...`
  Shortcuts in ``$CHPL_HOME/util/devel`` to grep the compiler, runtime, and
  Chapel module sources, resp.
