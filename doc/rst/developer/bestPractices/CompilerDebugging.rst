===============================
Tips On Debugging The Compiler
===============================

``~/.gdbinit``: ``CompilerIRTricks.rst``, ``GeneratedCode.rst``.
``~/.gdbinit``: compiler overview in ``$CHPL_HOME/doc/rst/developer/compilerOverview``.

With ``--print-passes``, the compiler prints individual passes as they progress.

We typically debug the compiler with ``gdb``.




Starting GDB and setup
-----------------------

* the easiest:  ``chpl --gdb <other compiler argument as usual>``

  This fires up ``gdb`` and sets it up with useful shortcuts
  (by reading in ``$CHPL_HOME/compiler/etc/gdb.commands``).

  Typing ``C-x 2`` within gdb should split the window, showing the source code.

* traditional:  ``gdb $CHPL_HOME/bin/$CHPL_HOST_PLATFORM/chpl``

* from within emacs:  ``M-x gdb <enter>``
  then provide the path to ``chpl``, e.g. ``/users/vass/chapel/bin/linux64/chpl``

  This engages emacs's ``gud`` mode, providing e.g. convenient access
  to compiler source code at ``breakpoints/stepping/stack`` unwinding
  and ``browsing/searching`` convenience of an emacs buffer.

Unless you start with ``chpl --gdb``, you need to set up gdb by hand.
Place the following in your ``~/.gdbinit``:

.. code-block:: bash

     ###########
     # short for "set up for chapel"
     define schp
     echo setting for Chapel in $arg0 \n
     set environment CHPL_HOME $arg0         # needed if not already in environment
     set environment CHPL_DEVELOPER 1        # ditto
     source $arg0/compiler/etc/gdb.commands  # done automatically by 'chpl --gdb'
     sdirs $arg0
     end

    # needed (I think) only for debugging the generated code
    define sdirs
     directory $arg0/modules/internal
     directory $arg0/modules/standard
     directory $arg0/modules/dists
     directory $arg0/modules/layouts
     end
    ###########


then call ``schp`` from the gdb prompt (do not use ``~`` or ``$CHPL_HOME``), e.g.:

(gdb) ``schp /users/vass/chapel``
setting for Chapel in /users/vass/chapel
(gdb)

(Advanced: create a shortcut for this command, for faster typing.)


Other useful shortcuts/setup
------------------------------

Here are some other useful additions to your ``~/.gdbinit``:

.. code-block:: bash

    ###########
    set print pretty
    set print object on
    set unwindonsignal on
    ###########


The ``set print object on`` makes ``gdb`` print a C++ object based on its
dynamic, rather than static, type. E.g. if the variable ``p`` has the
(static) type ``Expr*``, then ``print *p`` in gdb will include the fields
corresponding to whatever subclass of Expr 'p' is pointing to at the
moment (e.g. DefExpr, SymExpr, etc.).


TUI mode
---------

GDB has the ``TUI`` mode that provides for convenient viewing of the
source code during debugging, among other things, when running GDB in
a terminal. It is somewhat similar to Emacs\'s ``gud`` mode.

Turn the TUI mode on/off using one of: ``C-x C-a``, ``C-x a``, ``C-x A``.
Some additional information is, for example, here:

  http://davis.lbl.gov/Manuals/GDB/gdb_21.html


AST viewing and other compiler helper functions
------------------------------------------------

They are defined mostly in ``compiler/AST/view.cpp``
with corresponding shortcuts in ``compiler/etc/gdb.commands``:

.. code-block:: bash

  legend: C function # gdb shortcut

  print_view(BaseAST*)  # view, vi    # not showing ID
  nprint_view(BaseAST*) # nview, nv
  iprint_view(int id)   # iview, iv   # for the given ID
  list_view(BaseAST*)   # lview, lv   # looks somewhat like Chapel code



  #various ways to print out the AST subtree at the given node#

  viewFlags(Symbol*)    # flags
    #prints the flags set on this Symbol

  BaseAST::stringLoc()  # loc
    #prints the ast\'s location in the source code

  printCallStack()
    #prints the source code call stack leading to the part of the program where
    #the compiler is currently working, esp. during name/function resolution (?)

  aid(int id)
    #give the pointer to the AST node with the given ID




Other tips
-----------

.. code-block:: bash

  (gdb) lv userModules.v[0]
    prints the entire user module
    can search the output for a symbol name or ast ID
    if multiple user modules, do .v[1] etc. (right?)

  (gdb) break normalize
    stop right before the normalization pass

  (gdb) break checkNormalized
    stop right after the normalization pass

  gdbShouldBreakHere()
    compiler/etc/gdb.commands sets a breakpoint on this function


E.g. it is invoked when a compiler emits an error (or is about to die
for another reason) and for ``--break-on-id``.
You can call it in your debugging instrumentation as well.


Examining the source code
-------------------------

TAGS
  Is created when you build the compiler, in ``$CHPL_HOME/compiler/TAGS``
  - for use with emacs\'s find-tag et al.
  TAGS are also created elsewhere when building the runtime library
  and compiling a Chapel program.

BROWSE
  Created like TAGS above. Reflects the class hierarchy within the compiler,
  ebrowse\'s ``ebrowse-tree``.
  Presently there is little more than the ``BaseAST`` and ``Vec<>`` hierarchies.

- ``grepcomp``
- ``greprt``
- ``grepmod``

...
  Shortcuts in ``$CHPL_HOME/util/devel`` to grep the compiler, runtime, and
  Chapel module sources, resp.


Troubles exiting
----------------

Sometimes when debugging using the ``--gdb flag``, you will not be able to
execute the ``run`` or ``quit`` commands, typically due to insufficient
stack space.  The cause of the problem is that we alias these commands
to make a function call to clean up our ``/tmp`` directory space before
re-running or exiting, so if that stack space is not available, things
get stuck...  In such cases, you can use the ``rerun`` or ``halt`` aliases
that we set up to bypass the attempt to clean up ``/tmp`` and simply do
the run or quit alone (which requires no stack space).
