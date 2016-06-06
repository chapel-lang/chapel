.. _chapelhome-quickstart:

Chapel Quickstart Instructions
==============================

The following instructions are designed to help you build Chapel from
source.  If you haven't already downloaded Chapel, obtain the latest
release from http://chapel.cray.com/download.html, or refer to
http://chapel.cray.com/install.html for other installation options.

In the following instructions, note that building and using Chapel as
described in steps 1-7 disables optional and advanced features in the
interest of getting you a clean build as quickly as possible.  Step 8
explains how to re-build in the preferred configuration with these
additional features enabled.  Step 9 explains how to build Chapel for
distributed memory execution.


0) See `doc/prereqs.rst`_ for more information about system tools and
   packages you may need to have installed to build and run Chapel.

.. _doc/prereqs.rst: http://chapel.cray.com/docs/1.13/usingchapel/prereqs.html


1) Make sure that your shell is in the directory containing this
   ``QUICKSTART.rst`` file.  For example:

        ``cd ~/chapel-1.13.1``


2) Set up your environment to use Chapel in "Quick Start" mode:

======================================== ==========================================
**If you use:**                           **then type:**
---------------------------------------- ------------------------------------------
a bash-compatible shell (bash/zsh)       ``source util/quickstart/setchplenv.bash``
a csh-compatible shell (csh/tcsh)        ``source util/quickstart/setchplenv.csh``
the fish shell (fish)                    ``. util/quickstart/setchplenv.fish``
the Bourne shell (sh)                    ``. util/quickstart/setchplenv.sh``
======================================== ==========================================

   Note that there is no requirement to use these scripts long-term,
   they are merely designed to help new users get their environment
   variables and paths set up quickly.  Long-term, users may want to
   copy and paste these settings into their dotfiles or set them in
   other ways.  See `doc/chplenv.rst`_ for a complete description of
   Chapel's environment variables and their expected values.

.. _doc/chplenv.rst: http://chapel.cray.com/docs/1.13/usingchapel/chplenv.html


3) Build the compiler and runtime libraries using:

        ``gmake``

   or if your default make is GNU make compatible (as on Mac OS X or
   Cygwin), use:

        ``make``

   Parallel builds (e.g. ``gmake -j``) are also supported.


4) ``csh``/``tcsh`` users only: Update your shell's path hash table using:

        ``rehash``


5) Optionally, check that your Chapel installation is working correctly:

        ``gmake check``

   or:

        ``make check``


6) Compile an example program:

        ``chpl -o hello examples/hello.chpl``


7) Run the resulting executable:

       ``./hello``


8) Experiment with Chapel in this Quickstart mode to your heart's
   content.  Once you are comfortable with Chapel and interested in
   using a full-featured version in the preferred configuration:

   a) Open up a new shell to avoid inheriting the previous environment
      settings.

   b) Repeat steps 1-7 above, but in Step 2, source ``util/setchplenv.*``
      instead of ``util/quickstart/setchplenv.*``

   This will set up your environment to use Chapel in the preferred
   configuration.  Building this configuration involves compiling
   third-party packages, which will increase the overall build time.
   If you run into any portability issues, please let us know via
   http://chapel.cray.com/bugs.html.


9) All of the instructions above describe how to run Chapel programs
   in a single-locale (shared-memory) mode. To run using multiple
   locales (distributed memory), please refer to `doc/multilocale.rst`_.

.. _doc/multilocale.rst: http://chapel.cray.com/docs/1.13/usingchapel/multilocale.html


10) If you plan to do performance studies of Chapel programs, be sure
    to (a) use the full-featured version from steps 8-9 and (b) read
    ``$CHPL_HOME/PERFORMANCE.md`` to avoid common pitfalls.



What's next?
------------
=============================================== =========================
**For more detailed information about:**        **refer to:**
----------------------------------------------- -------------------------
    platform-specific notes                     `platforms`_
    online Chapel Documentation                 `chapel.cray.com/docs`_
    example Chapel programs                     ``examples/README``
    setting Chapel environment variables        `doc/chplenv.rst`_
    building the compiler                       `doc/building.rst`_
    compiling Chapel programs                   `doc/compiling.rst`_
    executing Chapel programs                   `doc/executing.rst`_
    debugging Chapel programs                   `doc/debugging.rst`_
    reporting bugs                              `doc/bugs.rst`_
    implementation status                       ``STATUS``
    performance status                          ``PERFORMANCE.md``
    Chapel modes for emacs and vim              ``highlight/README.md``
    Chapel's file and directory structure       ``README.files``
    changes since the last release              ``CHANGES.md``
=============================================== =========================

.. _platforms: http://chapel.cray.com/docs/1.13/platforms/index.html
.. _chapel.cray.com/docs: http://chapel.cray.com/docs/1.13/
.. _doc/building.rst: http://chapel.cray.com/docs/1.13/usingchapel/building.html
.. _doc/compiling.rst: http://chapel.cray.com/docs/1.13/usingchapel/compiling.html
.. _doc/executing.rst: http://chapel.cray.com/docs/1.13/usingchapel/executing.html
.. _doc/debugging.rst: http://chapel.cray.com/docs/1.13/usingchapel/debugging.html
.. _doc/bugs.rst: http://chapel.cray.com/docs/1.13/usingchapel/bugs.html
