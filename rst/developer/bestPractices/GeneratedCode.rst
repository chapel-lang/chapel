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
  var mytimer:Timer;
  mytimer.clear();

  mytimer.start();

  ... measured code goes here ...
  mytimer.stop();

  writeln("time taken: " + mytimer.elapsed() + " seconds");

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

      ``$CHPL_HOME/doc/rst/usingchapel/executing.rst``

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

We basically use ``gprof`` for profiling. (There is also ``gcov``.)


* Be sure your runtime is compiled with optimization (see above).


* When compiling, use:

      ``--ccflags -pg --ldflags -pg --fast --savec DIRECTORY``

  This produces `gprof-enabled` executable. (See `gprof` docs if unfamiliar.)

  You probably want ``--fast`` ``--savec``, but they do not affect profilability.


* If you want to profile the runtime as well, build it so:

 ::
      
      cd $CHPL_HOME/runtime
      make clean
      make DEBUG=0 OPTIMIZE=1 PROFILE=1

Note: currently you can have only one compilation of the runtime (see above).


Debugging/profiling the generated code, alternative approach
------------------------------------------------------------

You might find it more convenient to debug and profile (``gprof``/``gcov``)
*the same* version of the generated code.


* Keep track of how your runtime is presently built (see above).


* When compiling, use ``--savec`` but not ``-g`` (I think), ``--ccflags``, or ``--ldflags``,
  for example:

      ``--fast --savec DIR --c-line-numbers``

* Option A: run ``make -f DIR/Makefile`` then adjust the compilation
  commands being issued. (You might even be able to redirect the
  compilation to different runtime/lib directories.)


* Option B is to replace `DIR/Makefile`
  with the following (change ``a.out`` to your preferred executable name):

.. code-block :: bash


  ifneq ($(DB),)
  EXTR_FLAGS += -g
  EXTR_sfx += .db
  endif

  ifneq ($(GP),)
  EXTR_FLAGS += -pg
  EXTR_sfx += .gp
  endif

  ifneq ($(GC),)
  EXTR_FLAGS += -fprofile-arcs -ftest-coverage
  EXTR_sfx += .gc
  endif

  ifneq ($(DB),)
  # don't want OPT_CFLAGS
  COMP_GEN_CFLAGS = $(EXTR_FLAGS) $(WARN_GEN_CFLAGS)               $(NO_IEEE_FLOAT_GEN_CFLAGS)
  else
  COMP_GEN_CFLAGS = $(EXTR_FLAGS) $(WARN_GEN_CFLAGS) $(OPT_CFLAGS) $(NO_IEEE_FLOAT_GEN_CFLAGS)
  endif

  COMP_GEN_LFLAGS = $(EXTR_FLAGS) 
  BINNAME = a.out$(EXTR_sfx)
  TMPDIRNAME := $(dir $(lastword $(MAKEFILE_LIST)))
  TMPBINNAME = $(TMPDIRNAME)a.out.tmp
  CHAPEL_ROOT = $(CHPL_HOME)
  TAGS_COMMAND = -@which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && test -f $(CHAPEL_ROOT)/runtime/$(CHPL_TAGS_FILE) && cd $(TMPDIRNAME) && cp $(CHAPEL_ROOT)/runtime/$(CHPL_TAGS_FILE) . && $(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(CHPL_TAGS_APPEND_FLAG) *.c *.h

  CHPLSRC = $(TMPDIRNAME)_main.c 
  LIBS =

  include $(CHAPEL_ROOT)/runtime/etc/Makefile.include


Do all compilations in DIR (e.g. the directory with the generated code).

::

      make DB=1   # generates 'a.out.db' for debugging
      make GP=1   # generates 'a.out.gp' for gprof
      make GC=1   # generates 'a.out.gc' for gcov

For ``gcov``, you may need to rename a couple of files by hand:

::

      rm *.c.gcov *.gcda *.gcno
      make GC=1
      ./a.out.gc <whatever options you have for a.out>
      mv a.out{.tmp,}.gcda
      mv a.out{.tmp,}.gcno
      gcov a.out.gc


Profiling only parts of the runtime
-----------------------------------

Do you want ``gprof/gcov/...`` to look only at certain parts of the runtime,
to reduce profiling overhead, making the results more true to reality?

One way to do so is to transplant those parts from the runtime into
the generated code, patching everything involved to satisfaction
(e.g. so that those parts compile and the generated code refers to
them instead of the original runtime). This can easily be laborious,
but you get good control over what's going on.

Be sure NOT to re-run the Chapel compiler.



Miscellanea
-----------

`grepcomp`

`greprt`

`grepmod`

`...`
  Shortcuts in ``$CHPL_HOME/util/devel`` to grep the compiler, runtime, and
  Chapel module sources, resp.