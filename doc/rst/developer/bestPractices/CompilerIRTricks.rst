===============================
Examining/Debugging Compiler IR
===============================

See also: ``CompilerDebugging.txt``.

See also: compiler overview in ``$CHPL_HOME/doc/rst/developer/compilerOverview``.

An easy way to examine the evolution of the source code is to compile with
``--html``. For example, when you compile ``hello.chpl`` with this flag in conjunction
with ``--savec`` (useful for reference if you're trying to understand where a
given line of code comes from):
::

# chpl --html -o ./bin/hello ./test/release/examples/hello.chpl --savec ./Hello.Out


...you create a directory called ``log/`` in the current directory. (This trick
also can be spawned using ``-t``.) Note that you can change the defaulted log
directory by also using ``--log-dir``.

Within this directory contains a file called ``index.html``. Point your web
browser here.

You will see files corresponding to each generated .c file after every pass
of the compiler. For example, after the parser (pass 1) handles hello.c, you
should see output like this:


:: 

  module hello[70159]
   function chpl__init_hello[70160] ( ) : void[17]
      {
            {
                 (70157 call writeln "Hello, world!")
            }
      }

As the passes make the code more sophisticated, you will see more calls,
declarations, etc, and code can move or be removed entirely. Using the output
from ``--savec``, you may line up interesting code samples to see how the compiler
changes this code over time.

A key debugging concept is the unique ID associated with each AST node.
IDs are given sequentially to the nodes when they are constructed.
See the Compiler Overview document for more details.

The IDs are commonly shown in debugging and HTML printouts. In the
above example ::
 
 17 -> the void type, 70157 -> the call to writeln, etc.


(This call gets a function associated with it within the ``resolve`` pass.)

IDs are often used like this while debugging:

* find the ID of an interesting node in the HTML or debugging printout

 (e.g. when printing another node with gdb commands ``nv`` or ``lv``, see ``CompilerDebugging.txt``), then

* break on the creation of that node (see below) to see when/why that happened.

You can also go between the ID and the node pointer:

::

  myNodePointer->id  // gives the ID
  aid(myID)          // gives the AST pointer

The intention is for the IDs to remain the same between different runs
of the compiler, so that you restart the compiler (with the same arguments)
to trace the ID you discovered in a previous run. (There are cases when
IDs change, which should be fixed.)

Potential gotcha: sometimes code is copied or moved, and the original
reference is removed. You may have to chase the ID back to a previous
or parent ID.

To break on the creation of a particular ID, use ``--break-on-id``:

.. code-block:: bash

  # chpl --gdb --break-on-id 70157 -o ./bin/hello ./test/release/examples/hello.chpl
  Using host libthread_db library "/lib64/libthread_db.so.1".
  Breakpoint 1 at 0x527ae0
  (gdb) run
  Starting program: chpl --break-on-id 70157 -o ./bin/hello ./test/release/example/hello.chpl
  Breakpoint 1, 0x0000000000527ae0 in gdbShouldBreakHere ()
  (gdb) 

Should you want to break on more than one ID without running gdb several
times, the solution is a little contrived. 

.. code-block:: bash

  (gdb) break checkid if ((id == 70157) || (id == 512306))
  Breakpoint 2 at 0x418122: file baseAST.cpp, line 182.
  (gdb) run

As gdb implements this inefficiently, you may want first to break on
the pass(es) where the desired IDs are created, to reduce the wait time.

Alternatively, start with ``--break-on-id`` on the lowest ID.  Once gdb
breaks on that, update the variable ``breakOnID`` with the next-lowest ID,
the next time with the next-next-lowest ID, etc.

.. code-block:: bash

  (gdb) set breakOnID = 512306
  (gdb) continue

(If you find yourself using this a lot, holler - we should really just
implement a multiple-ID counterpart to ``--break-on-id``.)

To save some time on html generation, try ``--html-user``.
This generates html only for user modules. In the case
of ``hello.chpl``, only hello will be displayed.