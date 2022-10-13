:title: Mason Guide: *Hello World*

.. _using-a-package:
        
Using a Mason Package
=====================

Once mason has been installed, you are all ready to get started using mason
packages! This section outlines a quick hands-on approach to getting started
with mason packages as dependencies in your own code.

A Mason Hello World!
~~~~~~~~~~~~~~~~~~~~

So, mason is installed and ``mason --help`` is working, showing the available
mason subcommands. What now?

First, let's create a new directory to house our mason project. This will give
us everything we need to start installing dependencies from the mason registry
while keeping our directory structure nice and clean.

  .. code-block:: sh


    mason new HelloPackage
    cd HelloPackage

At this point, mason will start prompting you for some basic information about
your package. Since this package is just for getting started, all that you need
to worry about for now is the package name.

You can name your package whatever you would like, as long as it is a valid
Chapel identifier, but for simplicity, let's name our package ``HelloPackage``
and then go with the default values for all other prompts, leaving them blank.

If everything has gone as planned, you should be met with the prompt:
"Initialized new library project: HelloPackage". Now, let's get installing
our first mason package...

To view all available packages, run the ``mason search`` command. This will
output a list of all packages registered to the mason registry, which can be
viewed at https://github.com/chapel-lang/mason-registry.

The package that we are interested in with this tutorial is the ``HelloWorld``
package. Let's signal to mason that we would like to install that package:

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

After double-checking that you are in the top-level directory of the mason
project that you've been working in (named ``HelloWorld/`` in this tutorial),
now we can install our dependency and build our program with the command:

.. code-block:: sh

  mason build

After running this command, the ``HelloWorld`` package will be installed
to ``MASON_HOME``, which defaults to ``~/.mason``, so, assuming ``MASON_HOME``
has not been modified, the source code of all mason dependencies can be found
at ``~/.mason``.

If everything has worked up until now, it's time to run our program!

.. code-block:: sh

  mason run

You should see a "Hello Chapel friends from Mason" message printed to the
screen. Congratulations! You can now install mason dependencies and use them
in your code.

Hello World Tutorial Cheat Sheet
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. ``mason init``
2. ``mason add HelloWorld@1.0.0``
3. add ``use HelloWorld;`` and ``helloChapelFriends();`` to Chapel module in ``./src/`` directory
4. ``mason build``
5. ``mason run``
