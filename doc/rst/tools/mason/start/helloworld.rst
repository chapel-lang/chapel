.. _using-a-package:

Using a Mason Package
=====================

Once Mason has been installed, you are all ready to get started using Mason
packages! This section outlines a quick hands-on approach to getting started
with Mason packages as dependencies in your own code.

A Mason Hello World!
~~~~~~~~~~~~~~~~~~~~

So, Mason is installed and ``mason --help`` is working, showing the available
Mason subcommands. What now?

First, let's create a new directory to house our Mason project. This will give
us everything we need to start installing dependencies from the Mason registry
while keeping our directory structure nice and clean. You can name your
package whatever you would like, as long as it is a valid Chapel identifier,
but for simplicity, let's name our package ``HelloPackage``

  .. code-block:: sh


    mason new HelloPackage
    cd HelloPackage

If everything has gone as planned, you should now have a new directory called ``HelloPackage/`` with a basic ``Mason.toml`` file and some additional project
structure.

To view all available packages, run the ``mason search`` command. This will
output a list of all packages registered to the Mason registry, which can be
viewed at https://github.com/chapel-lang/mason-registry.

The package that we are interested in with this tutorial is the ``HelloWorld``
package. Let's signal to Mason that we would like to install that package:

  .. code-block:: sh

    mason add HelloWorld@0.1.0

This will add the ``HelloWorld`` package to our ``Mason.toml`` file as a
dependency (you can open the ``Mason.toml`` file to see the change).

Now, everything is ready to go with our ``HelloPackage`` program... now the
fun part: writing (or copying, in this case) some code.

Open up the package source file located at ``./src/HelloPackage.chpl``.
You should see something like this:

.. code-block:: chpl

  /* Documentation for HelloPackage */
  module HelloPackage {
    writeln("New library: HelloPackage");
  }

This program will run as-is, but we aren't using our ``HelloWorld`` dependency
just yet. In order to use the package, we need to add a ``use HelloWorld`` to
bring in the dependency. This gives us access to the whole module namespace
of the ``HelloWorld`` package, and we can call all of the available functions.

Let's call the ``helloChapelFriends()`` function to make sure that it worked.
Now, the code should look like this:

.. code-block:: chpl

  /* Documentation for HelloPackage */
  module HelloPackage {
    use HelloWorld;
    helloChapelFriends();
  }

At this point, everything should be good to go! Time to run the program.

After double-checking that you are in the top-level directory of the Mason
project that you've been working in (named ``HelloWorld/`` in this tutorial),
now we can install our dependency and build our program with the command:

.. code-block:: sh

  mason build

After running this command, the ``HelloWorld`` package will be installed
to ``MASON_HOME``, which defaults to ``~/.mason``, so, assuming ``MASON_HOME``
has not been modified, the source code of all Mason dependencies can be found
at ``~/.mason``.

If everything has worked up until now, it's time to run our program!

.. code-block:: sh

  mason run

You should see a "Hello Chapel friends from Mason" message printed to the
screen. Congratulations! You can now install Mason dependencies and use them
in your code.

Hello World Tutorial Cheat Sheet
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. ``mason init``
2. ``mason add HelloWorld@1.0.0``
3. add ``use HelloWorld;`` and ``helloChapelFriends();`` to Chapel module in ``./src/`` directory
4. ``mason build``
5. ``mason run``
