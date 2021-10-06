/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

/*
Support launching and interacting with other programs.

.. warning::

    This module has been deprecated - please use :mod:`Subprocess` instead.

Using functions in this module, one can create a subprocess
and possibly capture its output. It is also possible to provide
input to a subprocess.


To start a subprocess, use :proc:`Subprocess.spawn` or
:proc:`Subprocess.spawnshell`.  To wait for the subprocess process to
finish, use the :proc:`Subprocess.subprocess.wait` or
:proc:`Subprocess.subprocess.communicate` functions.

This example program produces a listing of files in the current directory with
names that begin with ``test.`` by using the ``ls`` command. The
output will be mixed in with the Chapel program's output.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["ls", "test.*"]);
  sub.wait();

This version also runs the ``ls`` command but uses a pipe
to read the output from the ``ls`` command.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["ls", "test.*"], stdout=PIPE);

  var line:string;
  while sub.stdout.readline(line) {
    write("ls returned: ", line);
  }

  sub.wait();

Here is an example program that provides input to a subprocess in addition to
capturing its output.  This version uses the ``cat`` command, which just prints
back its input.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["cat"], stdin=BUFFERED_PIPE, stdout=PIPE);

  sub.stdin.writeln("Hello");
  sub.stdin.writeln("World");

  sub.communicate();

  var line:string;
  while sub.stdout.readline(line) {
    write("Got line: ", line);
  }

  // prints out:
  // Got line: Hello
  // Got line: World


Here is a final example in which the Chapel program uses 2 tasks
to work with a subprocess. One task is producing data and the
other task is consuming it.

.. code-block:: chapel

  use Spawn;

  var input = ["a", "b", "c"];

  var sub = spawn(["cat"], stdin=PIPE, stdout=PIPE);
  cobegin {
    {
      // one task writes data to the subprocess
      for x in input {
        sub.stdin.writeln(x);
      }
      // this close is important; otherwise the other task blocks forever
      sub.stdin.close();
    }

    {
      var line:string;
      while sub.stdout.readln(line) {
        writeln("Got line ", line);
      }
    }
  }
  sub.wait();

  // prints out:
  // Got line: a
  // Got line: b
  // Got line: c


.. note::

  Creating a subprocess that uses :const:`Subprocess.PIPE` to provide
  input or capture output does not work when using the ugni communications layer
  with hugepages enabled and when using more than one locale. In this
  circumstance, the program will halt with an error message. These scenarios do
  work when using GASNet instead of the ugni layer.

 */

deprecated "The Spawn module is deprecated. Please use Subprocess instead."
module Spawn {
  use Subprocess;
}
