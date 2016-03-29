.. _chapelhome-quickstart:

Chapel Quickstart Instructions
==============================

The following instructions are designed to help you build Chapel from
source.  If you haven't already downloaded Chapel, head over to
http://chapel.cray.com/download.html to obtain the latest release, or
see http://chapel.cray.com/install.html for other installation
options.

In the following instructions, note that building and using Chapel as
described in steps 1-7 disables optional and advanced features in the
interest of getting you a clean build as quickly as possible.  Steps
8-9 explain how to re-build the preferred configuration that has these
additional features enabled.


0) See `prereqs.rst`_ for more information about system tools and
   packages you may need to have installed in support of Chapel.


1) Make sure that your shell is in the directory containing this
   ``QUICKSTART.rst`` file.  For example:

        ``cd ~/chapel-1.13.0``


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
   make these settings in their dotfiles or via other means.  See
   `chplenv.rst`_ for a complete description of Chapel's environment
   variables and their values.

.. _chplenv.rst: http://chapel.cray.com/docs/1.13/usingchapel/chplenv.html


3) Build the compiler and runtime libraries using:

        ``gmake``

   or if your default make is GNU make compatible (as on Mac OS X or
   Cygwin), use:

        ``make``

   Parallel builds (e.g. ``gmake -j``) are also supported.


4) ``csh``/``tcsh`` users only: Update your shell's path cache using:

        ``rehash``


5) Optionally, check that your Chapel installation is working correctly:

        ``gmake check``

   or:

        ``make check``


6) Compile an example program using:

        ``chpl -o hello examples/hello.chpl``


7) Execute the resulting executable:

       ``./hello``


8) Experiment with Chapel in this Quick-Start mode to your heart's
   content.

   Once you are comfortable with Chapel and interested in using a
   full-featured version in the preferred configuration:

   a) Open up a new shell to avoid inheriting the previous environment
      settings.

   b) Repeat steps 1-7 above, but in Step 2, use ``util/setchplenv.*``
      instead of ``util/quickstart/setchplenv.*``

   This will set up your environment to use Chapel in the preferred
   configuration.  Building this configuration will involve compiling
   one or more third-party packages which will increase the overall
   build time.  If you run into any portability issues, please let us
   know at chapel_info@cray.com.


9) All of the instructions above describe how to run Chapel programs
   in a single-locale (shared-memory) mode. To run using multiple
   locales (distributed memory), please refer to `multilocale.rst`_.

.. _multilocale.rst: http://chapel.cray.com/docs/1.13/usingchapel/multilocale.html


10) If you plan to do performance studies of Chapel programs, be sure
    to (a) use the full-featured version and (b) read the ``PERFORMANCE.md``
    file in this directory to avoid common pitfalls.



What's next?
------------
=============================================== =====================================
**For more detailed information about:**        **refer to:**
----------------------------------------------- -------------------------------------
    changes since the last release              ``CHANGES.md``
    implementation status                       ``STATUS``
    performance status                          ``PERFORMANCE.md``
    Chapel's file and directory structure       ``README.files``
    setting Chapel environment variables        `chplenv.rst`_
    building the compiler                       `building.rst`_
    using the compiler                          `compiling.rst`_
    executing Chapel programs                   `executing.rst`_
    debugging Chapel programs                   `debugging.rst`_
    reporting bugs                              `bugs.rst`_
    Chapel modes for emacs and vim              ``highlight/README.md``
    example Chapel programs                     ``examples/README``
    a quick reference for Chapel syntax         `doc/quickReference.pdf`_
    the Chapel language specification           `doc/chapelLanguageSpec.pdf`_
    other Chapel Documentation                  `chapel.cray.com/docs`_
    third-party software that we use            ``third-party/README``
=============================================== =====================================

.. _doc/quickReference.pdf: http://chapel.cray.com/spec/quickReference.pdf
.. _doc/chapelLanguageSpec.pdf: http://chapel.cray.com/spec/spec-0.98.pdf
.. _chapel.cray.com/docs: http://chapel.cray.com/docs/1.13/
.. _building.rst: http://chapel.cray.com/docs/1.13/usingchapel/building.html
.. _compiling.rst: http://chapel.cray.com/docs/1.13/usingchapel/compiling.html
.. _executing.rst: http://chapel.cray.com/docs/1.13/usingchapel/executing.html
.. _debugging.rst: http://chapel.cray.com/docs/1.13/usingchapel/debugging.html
.. _bugs.rst: http://chapel.cray.com/docs/1.13/usingchapel/bugs.html


=============================================== =====================================
**To use Chapel on a specific platform:**       **refer to:**
----------------------------------------------- -------------------------------------
       a Cray system                            `cray.rst`_
       Cygwin over Windows                      `cygwin.rst`_
=============================================== =====================================


.. _cray.rst: http://chapel.cray.com/docs/1.13/platforms/cray.html
.. _cygwin.rst: http://chapel.cray.com/docs/1.13/platforms/cygwin.html
