.. _readme-macosx:

========================
Using Chapel on Mac OS X
========================

There are two main approaches for using Chapel on Mac OS X:

1) Install via Homebrew_.  For Homebrew users, this is the
   quickest way to get up and running, but it results in a copy of
   Chapel that only supports shared-memory (single-locale) executions.

2) Build Chapel from source, as with any other UNIX
   system.  This is slightly more involved, but supports Chapel's full
   feature set.

.. _macosx-homebrew:

--------
Homebrew
--------

Chapel can be installed through Homebrew_ with the following commands:

.. code-block:: bash

   brew update
   brew install chapel

These commands install the latest release of Chapel.  When using a
Homebrew installation of Chapel, the ``CHPL_HOME`` directory can be found by
running the following command:

.. code-block:: bash

   chpl --print-chpl-home

Compile and run a test program:

.. code-block:: bash

   chpl `chpl --print-chpl-home`/examples/hello.chpl
   ./hello

If you're new to Chapel, refer to the `What's Next?
<https://chapel-lang.org/docs/usingchapel/QUICKSTART.html#what-s-next>`_
section of :ref:`chapelhome-quickstart` for next steps.

.. note::

   It is also possible for developers to use Homebrew to build and
   install a development version based on the main branch on
   GitHub::

    brew install chapel --HEAD

Multi-locale Execution
~~~~~~~~~~~~~~~~~~~~~~

The Homebrew installation of Chapel supports 2 different ways of
:ref:`running multi-locale programs <readme-multilocale>` on your
shared memory machine:

* Using the :ref:`GASNet SMP conduit <readme-gasnet-smp>` allows you to run
  multi-locale programs by partitioning the machine's resources into multiple
  locales.

  Compile and run a test program:

  .. code-block:: bash

     chpl --comm=gasnet --comm-substrate=smp \\
       `chpl --print-chpl-home`/examples/hello6-taskpar-dist.chpl
     ./hello6-taskpar-dist -nl 4

* Using the :ref:`GASNNet UDP conduit <readme-gasnet-emulating-multilocale>`
  allows you to run multi-locale programs by oversubscribing the machine's
  resources.

  Compile and run a test program:

  .. code-block:: bash

     chpl --comm=gasnet --comm-substrate=udp \\
       `chpl --print-chpl-home`/examples/hello6-taskpar-dist.chpl
     chplrun-udp ./hello6-taskpar-dist -nl 4

  .. note::

     Note the usage of the ``chplrun-udp`` command to run the program. This is a
     necessary to setup the environment for the UDP conduit to properly launch
     the program locally through the network stack. You can also forgo the
     ``chplrun-udp`` command and run the program directly, making sure to properly
     setup the environment. This will also allow you to launch jobs on
     multiple nodes across the network. See :ref:`using-udp` for details.

For more information on the differences between these two modes,
see :ref:`readme-udp-vs-smp`.

GPU Execution
~~~~~~~~~~~~~

The Homebrew installation of Chapel comes with a mode for emulating GPU
execution. This allows you to test Chapel programs designed for a GPU, but
it does not provide actual GPU support. If you are interested in compiling
Chapel code for NVIDIA or AMD GPUs, see the
:ref:`GPU documentation <readme-gpu>`.

Compile and run a test program:

.. code-block:: bash

   chpl --locale-mode=gpu --gpu=cpu `chpl --print-chpl-home`/examples/gpu/hello-gpu.chpl
   ./hello-gpu

.. warning::

   Chapel does not yet support GPU commutation with Apple GPUs. The above code
   will emulate GPU execution by treating the CPU as a GPU.

.. _Homebrew: https://brew.sh/


--------------------
Building from Source
--------------------

If you are not using the Homebrew package, it will be necessary to
install a few development tools in order build Chapel. The instructions
below show one way of getting a working development environment.

First, install XCode_ or the developer Command Line Tools. XCode can be
installed from the App store. Or, you can install the developer Command
Line Tools by opening a Terminal window and running::

    xcode-select --install

Once this is done, open up a new Terminal window to verify that the
``clang`` and ``python3`` programs can be found and have a suitable
version (see also :ref:`readme-prereqs`) by running the following
commands::

    clang --version
    python3 --version

After this step, you should have a working C/C++ compiler, ``make`` support,
and a working installation of Python 3.

Next, install CMake_. Since XCode installation does not include CMake,
and it's required to build Chapel, it's necessary to install it
separately. You can install it using a source or binary release from the
CMake_ website. At the time of this writing, the CMake binary release
includes a graphical CMake.app but does not install the command-line
tool.  If you have CMake.app installed in your Applications folder, you
can ask it to install the command-line tool with the following command::

    sudo /Applications/CMake.app/Contents/bin/cmake-gui --install

Once this is done, open up a new Terminal window to verify that
``cmake`` is available and has a suitable version (see
:ref:`readme-prereqs`)::

    cmake --version

From here, follow the main documentation for building from source at
:ref:`chapelhome-quickstart`.

.. _Xcode: https://developer.apple.com/xcode/

.. _CMake: https://cmake.org/download/
