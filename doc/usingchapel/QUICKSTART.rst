.. _chapelhome-quickstart:

Chapel Quickstart Instructions
==============================

These instructions are designed to help you get started with
a source distribution of Chapel.

In the following instructions, note that building and using Chapel as
described in steps 2-5 disables optional and advanced features in the
interest of getting you a clean build as quickly as possible. Later 
sections explain how to re-build in the preferred configuration and how to
enable more features, such as distributed memory execution.


0) See `doc/prereqs.rst`_ for more information about system tools and
   packages you may need to have installed to build and run Chapel.

1) If you don't already have Chapel 1.14, see
   http://chapel.cray.com/download.html .

2) If you are using a source release, build Chapel in a *quickstart*
   configuration. The *quickstart* script used below is designed to help you
   get started with Chapel. After that, you may want to switch to a more
   full-featured configuration. See using-a-more-full-featured-chapel_ below.

   a. Expand the source release if you haven't already:

      .. code-block:: bash

         tar xzf chapel-1.14.0.tar.gz

   b. Make sure that your shell is in the directory containing
      QUICKSTART.rst, for example:

      .. code-block:: bash

         cd chapel-1.14.0

   c. Set up your environment for Chapel's Quickstart mode.
      If you are using a shell other than bash,
      see quickstart-with-other-shells_ below.

      .. code-block:: bash

         source util/quickstart/setchplenv.bash

   d. Use GNU make to build Chapel.
      On some systems, you will have to use gmake.
      See `doc/building.rst`_ for more information about building Chapel.

      .. code-block:: bash

         make

   e. Optionally, check that your Chapel build is working correctly

      .. code-block:: bash

         make check


3) Compile an example program:

   .. code-block:: bash

      chpl -o hello examples/hello.chpl

4) Run the resulting executable:

   .. code-block:: bash

      ./hello

5) Experiment with Chapel in this Quickstart mode to your heart's
   content.  Once you are comfortable with Chapel and interested in
   using a full-featured version in the preferred configuration, see
   the next section.


.. _using-a-more-full-featured-chapel:

Using a More Full-Featured Chapel
---------------------------------

To use Chapel in a more full-featured and preferred configuration,
you will need to rebuild Chapel from source in a different configuration.

*  Open up a new shell to avoid inheriting the previous environment
   settings.

*  Repeat steps 2-5 above, but in Step 2, source ``util/setchplenv.bash``
   instead of ``util/quickstart/setchplenv.bash``.
   This will set up your environment to use Chapel in the preferred
   configuration.  Building this configuration involves compiling
   third-party packages, which will increase the overall build time.
   If you run into any portability issues, please let us know via
   `doc/bugs.rst`_.

   .. code-block:: bash

      # Set environment variables to preferred configuration
      source util/setchplenv.bash

      # re-build Chapel
      make

      # make check is available but optional
      make check

      # compile a sample program
      chpl -o hello examples/hello.chpl

      # run the sample program
      ./hello

   At this point, you can choose to continue using ``setchplenv.bash``
   or use another method to configure Chapel. In particular, you might
   use a :ref:`chplconfig <readme-chplenv.chplconfig>` file or
   set environment variables your in dot files or your own script.

   See `doc/chplenv.rst`_ for a complete description of
   Chapel's configuration variables, what they mean, and how they
   can be set.


Using Chapel in Multi-Locale Mode
---------------------------------

All of the instructions above describe how to run Chapel programs
in a single-locale (shared-memory) mode. To run using multiple
locales (distributed memory), please refer to `doc/multilocale.rst`_.

Performance
-----------

If you plan to do performance studies of Chapel programs, be sure to use the
full-featured version from using-a-more-full-featured-chapel_ above and read
``$CHPL_HOME/PERFORMANCE.md`` to avoid common pitfalls.


.. _quickstart-with-other-shells:

Quickstart with Other Shells
----------------------------

==================================== ==========================================
**If you use:**                       **then type:**
------------------------------------ ------------------------------------------
a bash-compatible shell (bash/zsh)   ``source util/quickstart/setchplenv.bash``
a csh-compatible shell (csh/tcsh)    ``source util/quickstart/setchplenv.csh``
the fish shell (fish)                ``. util/quickstart/setchplenv.fish``
the Bourne shell (sh)                ``. util/quickstart/setchplenv.sh``
==================================== ==========================================


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
    performance status                          ``PERFORMANCE.md``
    Chapel modes for emacs and vim              ``highlight/README.md``
    Chapel's file and directory structure       ``README.files``
    changes since the last release              ``CHANGES.md``
=============================================== =========================

.. _doc/prereqs.rst: prereqs.html
.. _doc/multilocale.rst: multilocale.html
.. _platforms: ../platforms/index.html
.. _chapel.cray.com/docs: http://chapel.cray.com/docs/1.14/
.. _doc/chplenv.rst: chplenv.html
.. _doc/building.rst: building.html
.. _doc/compiling.rst: compiling.html
.. _doc/executing.rst: executing.html
.. _doc/debugging.rst: debugging.html
.. _doc/bugs.rst: bugs.html
