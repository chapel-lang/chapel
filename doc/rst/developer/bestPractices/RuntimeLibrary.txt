==========================
The Chapel Runtime Library
==========================

Where is it?
------------

Sources:
  $CHPL_HOME/runtime/src
  $CHPL_HOME/runtime/include

Compiled code:
  $CHPL_HOME/runtime/src/gen/MORE-DIRS
  $CHPL_HOME/lib/MORE-DIRS

  MORE-DIRS indicate the various aspects of the configuration
  (e.g. which comm/task/threads layers were used, runtime platform, etc.)

See also:
  $CHPL_HOME/runtime/README


Compiling
---------

Currently you can have only one compilation of the runtime (per configuration)
in terms of optimization, debugging support, and profiling:

      make DEBUG=1 OPTIMIZE=0   # debug support
      make DEBUG=0 OPTIMIZE=1   # optimized
      make DEBUG=0 OPTIMIZE=1 PROFILE=1   # profiling support
      make DEBUG=0 OPTIMIZE=1 WARNINGS=1  # promote backend C compiler warnings to errors


Debugging
---------

See also debugging notes in GeneratedCode.txt.


Examining the source code
-------------------------

$CHPL_HOME/runtime/TAGS
  generated when you build the runtime
  also included in TAGS for a compiled Chapel program
