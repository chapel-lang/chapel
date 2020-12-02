:title: Users Guide: Hello world

Hello world: simple console output
==================================

A good feature to learn first in any language is how to print things
out to the console.  Although Chapel has a sophisticated set of I/O
features, let's just focus on a simple capability to get started.


The ``writeln()`` procedure prints its list of comma-separated
arguments to the console, one after the other without spaces, followed
by a newline.  Thus, the following statement will print a greeting to
the console:

.. literalinclude:: examples/users-guide/base/hello.chpl
  :caption:
  :language: chapel
  :lines: 1-1

This guide will cover the Chapel program structure in more detail later;
for now, it's sufficient to know that a Chapel program can simply be a
sequence of statements in a file.  Thus, the statement above is a
complete Chapel program.  If you type or paste it into a text editor
and save it into a file, say ``hello.chpl``, you'll have written your
first Chapel program.

Given a working Chapel compiler (see `Downloading Chapel
<https://chapel-lang.org/download.html>`_ for details), you can compile
the program using::

    chpl hello.chpl -o hello

which says to compile the ``hello.chpl`` Chapel source file, naming
the output binary ``hello``.  Once the compiler is done, you can run
the program using::

    ./hello

The program should then print the greeting to your console as follows:

.. literalinclude:: examples/users-guide/base/hello.good
  :language: text

And with that, you've executed your first Chapel program!
