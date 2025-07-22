/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

   .. note::

    You must declare your ``main`` function to take an array of string arguments
    to use the ``Help`` module. See the examples below or the technical note about
    :ref:`getting arguments from main()<technote-mainWithArgs>` for more
    information on this feature.

   Chapel programs can declare main to take arguments like this:

   .. literalinclude:: ../../../../test/library/standard/Help/doc-examples/example_read_args.chpl
    :language: chapel
    :start-after: START_EXAMPLE
    :end-before: STOP_EXAMPLE

   Programs that use this feature might need to expand upon the usage message
   that explains which config variables are available. To do so, this module
   includes the :proc:`printUsage` function.
 */
@unstable("The 'Help' module is unstable")
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

     .. literalinclude:: ../../../../test/library/standard/Help/doc-examples/example_print_usage.chpl
      :language: chapel
      :start-after: START_EXAMPLE
      :end-before: STOP_EXAMPLE

     .. note::
        A Chapel program's executable name (the 0-th argument to main) will
        vary between multilocale and single locale settings.  See
        :ref:`readme-multilocale` for more details.
   */
  proc printUsage() {
    extern proc printHelpTable();
    extern proc printConfigVarTable();

    printHelpTable();
    printConfigVarTable();
  }
}
