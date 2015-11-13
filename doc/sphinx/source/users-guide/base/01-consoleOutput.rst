:title: Users Guide: Simple Console Output: Hello world

Simple Console Output: Hello world
==================================

A good thing to learn first in any language is how to print things out
to the console.  Although Chapel has a sophisticated set of I/O
features, for now let's just focus on simple uses of the ``writeln()``
routine to get started.  It prints its list of comma-separated
arguments to the console, followed by a newline.  Thus, the following
statement will print a greeting to the console:

.. literalinclude:: ../../../../../test/release/examples/guide/base/01-hello.chpl
  :language: chapel
  :lines: 1-1

We'll cover program structure in more detail later; for now, it's
sufficient to know that a Chapel program can simply be a list of
statements in a file.  Thus, the statement above is a complete Chapel
program.  If you type or paste it into a text editor and save it into
a file (say ``hello.chpl``), you'll have written your first Chapel
program.

Given a working Chapel compiler (see `Installing Chapel
<http://chapel.cray.com/install.html>`_ for details), you can compile
the program using::

    chpl hello.chpl -o hello

which says to compile the ``hello.chpl`` Chapel source file, naming
the output binary ``hello``.  Once the compiler is done, you can run
the program using::

    ./hello

The program should then print the greeting to your console as follows:

.. literalinclude:: ../../../../../test/release/examples/guide/base/01-hello.good
  :language: text

Note that writeln() takes an arbitrary number of arguments and will
print them to the console one after the other without spaces, followed
by a single linefeed.
