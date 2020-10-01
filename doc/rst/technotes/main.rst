.. _readme-main:

================
main() Functions
================

After running module initialization code (top-level statements in
modules), a Chapel program will start executing from the entry point,
a function named main().  Chapel's main() function has typically taken
no arguments and had no return type.  This README describes initial
support for returning a status value and making main() take arbitrary
command-line arguments.



Returning a status value
========================
Traditionally, the only way to return a nonzero exit value from a
Chapel program has been to call exit().  Optionally, main() can be
declared to return an integer in which case the value returned serves
as the status value.



Having main() accept command-line arguments
===========================================

Overview
--------
Because Chapel programs have traditionally accepted a number of
"built-in" flags and configuration variables, there has been no way to
create a Chapel program that accepts arguments like an arbitrary list
of filenames or flags that might conflict with built-in capabilities.
For most uses, Chapel's existing config variable support is useful and
easy; occasionally, however, one wants to fully control argument
processing.

If a program's main() function is declared to take no arguments, Chapel
will behave as it traditionally has -- built-in flags and config
settings will be handled and unrecognized flags will trigger an error.

Alternatively, a main() function can be declared to take an array of
strings as an argument.  By doing so, the program opts-in to explicit
argument handling, described in the remainder of this README.


Usage
-----
To use this feature, declare main as follows:

.. code-block:: chapel

  proc main(args: [] string) {
    // ...body...
  }

When main is so declared, Chapel's default command line argument
processing changes. Rather than generate an error message when an
unknown argument is encountered, that argument will instead be passed
on to this main(args) function. Arguments corresponding to config
variables or Chapel's predefined flags (like ``-nl``, ``-b``, etc) will still
be processed and will not be passed on to the main(args) function.

To be clear, the arguments that would be handled without using this
feature will continue to behave as usual. The difference is that other
arguments will be passed to main(args) rather than generating an error
message. The only exception to this, is ``--help`` and ``-h``.

Prior to this feature, a Chapel program would always exit immediately
after printing the help message. A programmer might want to print
other information explaining what non-config-var command line
arguments are available and what they might do. For this reason, when
a Chapel program includes a main declared to take arguments, ``--help``
and ``-h`` will be passed on to the main(args) function.  The function
Help.printUsage() will print out the familiar config var table.

To gain access to this new function requires a module:

``use Help;``

From that module, the function printDefaultUsage() is available. For
example, consider the following program:

.. code-block:: chapel

  use Help;

  proc main(args: [] string) {
    for a in args {
      if a=="--help" {
        printUsage();
        writeln("\nEXTRA ARGUMENTS:");
        writeln(  "================");
        writeln("You can provide an arbitrary list of filenames.");
        exit(0);
      }
    }
  }

This program scans its argument list for ``--help``, and if found, it
prints the standard usage message as well as some additional text
explaining the purpose of the extra arguments. Note that this could be
used by a program which simply wants to add explanatory text to the
``--help`` output without actually handling any additional arguments.

Finally, as is common in GNU programs, there is a way to suspend
argument processing when main is declared to take arguments. Suppose
you are writing ``rm`` in Chapel and need to delete a file named
``--memTrack=true``. Your program will consume this flag during startup,
rather than passing it on to the declared main function. However,
placing the argument ``--`` in the argument list will cause all
following arguments to be passed to the declared main function and
will suspend handling of any builtin command line
arguments. Specifically, if ``myprogram`` is a Chapel program which has
declared main to take arguments:

``./myprogram -- --memTrack=true``

will result in the program receiving ``--memTrack=true`` as the first
argument after the executable name.



