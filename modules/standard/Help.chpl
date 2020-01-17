/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Functions for producing program help and usage.

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
 */
module Help {

  /*
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
   */
  proc printUsage() {
    extern proc printHelpTable();
    extern proc printConfigVarTable();

    printHelpTable();
    printConfigVarTable();
  }
}
