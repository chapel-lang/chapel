.. _readme-macosx:

========================
Using Chapel on Mac OS X
========================

There are two main approaches for using Chapel on Mac OS X:

1) Install via Homebrew_.  For Homebrew users, this is the
   quickest way to get up and running, but it results in a copy of
   Chapel that only supports shared-memory (single-locale) executions.

2) Build Chapel from source, as with any other UN*X
   system.  This is slightly more involved, but supports Chapel's full
   feature set.

--------
Homebrew
--------

A minimal installation of Chapel can be obtained through Homebrew_ with the
following commands::

    brew update
    brew install chapel

These commands install the latest release of Chapel.  When using a
Homebrew installation of Chapel, the ``CHPL_HOME`` directory is here::

    `brew --cellar`/chapel/<chapel-version>/libexec/

Compile and run a test program::

    chpl `brew --cellar`/chapel/<chapel-version>/libexec/examples/hello.chpl
    ./hello

If you're new to Chapel, refer to the `What's Next?
<https://chapel-lang.org/docs/usingchapel/QUICKSTART.html#what-s-next>`_
section of :ref:`chapelhome-quickstart` for next steps.

.. note::

   The Homebrew installation provides a minimal installation of Chapel
   for users to explore and test the language.  Of the omitted
   features, :ref:`multilocale <readme-multilocale>` support is most
   notable.  Users interested in utilizing Chapel's complete set of
   features should build Chapel from source (option 2 above).

.. note::

   It is also possible for developers to use Homebrew to build and
   install a development version based on the main branch on
   GitHub::

    brew install chapel --HEAD


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
