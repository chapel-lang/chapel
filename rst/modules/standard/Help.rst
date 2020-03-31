.. default-domain:: chpl

.. module:: Help
   :synopsis: Functions for producing program help and usage.

Help
====
**Usage**

.. code-block:: chapel

   use Help;

Functions for producing program help and usage.

Chapel programs can declare main to take arguments like this:

.. code-block:: chapel

  proc main(args: [] string) {
    for a in args {
      // process arguments
      writeln("Got argument ", a);
    }
  }

See :ref:`doc/rst/technotes/main.rst <readme-main>` in a Chapel release for
more information on this feature.

Programs that use this feature might need to expand upon the usage message
that explains which config variables are available. To do so, this module
includes the :proc:`printUsage` function.


.. function:: proc printUsage()

   
   Print out a usage message for config variables. This function does not
   exit. A typical use of this function would be to call it when ``"--help"``
   or ``"-h"`` are encountered when doing custom argument processing. In
   addition to calling this function, programs with custom argument
   processing should probably also print out a description of the arguments
   that they take.  Once both help messages have been output, the program
   would normally exit early - for example, by returning from main.
   
   A typical example might be this program, that accumulates a list of
   filenames in addition to handling config variable arguments:
   
   .. code-block:: chapel
   
     use Help;
   
     config const x = 5;
   
     proc main(args: [] string) {
       var filenames: [1..0] string;
   
       var programName = args[0];
   
       for a in args[1..] {
         if a == "-h" || a == "--help" {
           writeln("Usage: ", programName, " <options> filename [filenames]");
           printUsage();
           exit(1); // returning 1 from main is also an option
         } else {
           filenames.push_back(a);
         }
       }
   
       writef("Got filenames = %ht\n", filenames);
       writef("Got configuration variable x = %ht\n", x);
     }
   

