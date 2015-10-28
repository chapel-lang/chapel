:title: Users Guide: Simple Console Output: Hello world

Simple Console Output: Hello world
==================================

Perhaps the first thing to learn in any language is how to print
things out to the console.  Although Chapel has a sophisticated set of
I/O features, for now we'll just focus on the writeln() routine to get
started.  It prints is argument(s) to the console, followed by a
newline.  Thus, the following statement will print a greeting to the
console:

.. code-include::../code/01-hello.chpl
  :lexer: chapel

We'll talk in more detail about program structure later, but for now,
it's sufficient to know that a Chapel program can simply be a list of
statements in a file.  Thus, if you type the code above into a text
editor and save it into a file named hello.chpl, you've written your
first Chapel program.  

Given a working Chapel compiler (see
http://chapel.cray.com/install.html for details), you can compile the
program using:

    chpl hello.chpl -o hello

which says to compile the hello.chpl source file and name the output
binary 'hello'.  Once the compiler is done, you can run the program
using:

    ./hello

which should print the greeting to your console as follows:

.. code-include::../code/01-hello.good

Note that writeln() takes an arbitrary number of arguments and will
print them to the console one after the other without spaces, followed
by a single linefeed.
