.. _chapelhome-quickstart:

Chapel Quickstart Instructions
==============================

These instructions are designed to help users get started with a
single-locale (shared-memory) implementation of Chapel from the source
distribution of the release as quickly as possible.  If you want to
understand Chapel's configuration options, build process, and
installation more thoroughly, please refer to :ref:`readme-chplenv`
and :ref:`readme-building` instead.

These instructions first have you build a minimal, stripped-down
version of Chapel to reduce build times and the potential for
third-party portability issues.  Once you are interested in a
full-featured version of Chapel, refer to
:ref:`using-a-more-full-featured-chapel` below.


0) See :ref:`readme-prereqs` for information about system tools and
   packages you should have available to build and run Chapel.


1) If you don't already have the Chapel 1.25 source release, see
   https://chapel-lang.org/download.html.


2) Build Chapel in its 'Quickstart' configuration:

   a. Unpack the source release if you haven't already:

      .. code-block:: bash

         tar xzf chapel-1.25.1.tar.gz

   b. Make sure that you are in the directory that was created when
      unpacking the source release, for example:

      .. code-block:: bash

         cd chapel-1.25.1

   c. Set up your environment for Chapel's Quickstart mode.
      If you are using a shell other than ``bash`` or ``zsh``,
      see :ref:`quickstart-with-other-shells` below.

      .. code-block:: bash

         source util/quickstart/setchplenv.bash

   d. Use GNU make to build Chapel.  On some systems, you may have to
      use ``gmake`` if ``make`` is not a GNU version.

      .. code-block:: bash

         make


3) Compile an example program, which uses a ``forall`` loop to print messages:

   .. code-block:: bash

      chpl examples/hello3-datapar.chpl


4) Run the resulting executable, which will print 100 messages in parallel:

   .. code-block:: bash

      ./hello3-datapar


5) Experiment with Chapel in this Quickstart mode to your heart's
   content.  If you'd like to use this build of Chapel in a different
   shell / terminal session, see :ref:`using-chapel-in-another-shell`
   below.  Once you are comfortable with Chapel and interested in
   using a full-featured version in the preferred configuration, see
   the next section.


.. _using-a-more-full-featured-chapel:

Using Chapel in its Preferred Configuration
-------------------------------------------

To use Chapel in its preferred, full-featured mode, you will need to
rebuild Chapel from source in a different configuration:

* Open up a new shell to avoid inheriting the previous environment
  settings.

* The Quickstart configuration described above sets ``CHPL_LLVM=none``
  for simplicity and to save time.  This causes the Chapel compiler to
  use its C back-end, which is not the preferred option; as of Chapel
  1.25, LLVM is the default back-end, which needs to be available for
  full functionality.  There are a few options for using LLVM:

  - ensure that you have a compatible version of LLVM installed on
    your system and set ``CHPL_LLVM=system`` (or leave it unset and
    Chapel should find it if it's in your path.) Currently compatible
    versions are LLVM-11 and LLVM-12.

  - set ``CHPL_LLVM=bundled`` to have Chapel build and use the bundled
    version of LLVM (note that building the bundled version of LLVM
    can take a long time and requires CMake version 3.13.4 or higher)

  - set ``CHPL_LLVM=none`` to continue using the C back-end rather
    than LLVM

* If you are interested in building Chapel to support multiple compute
  nodes (locales), refer to :ref:`readme-multilocale` for other
  settings to enable that.
    
* Repeat steps 2-5 above, but in step 2, source
  ``util/setchplenv.bash`` instead of
  ``util/quickstart/setchplenv.bash``.  This will set up your
  environment to use Chapel in the preferred configuration.  Note that
  building this configuration involves compiling third-party packages,
  which will increase the overall build time.

  .. code-block:: bash

     # Set environment variables to preferred configuration
     source util/setchplenv.bash

     # re-build Chapel
     make

     # compile a sample program
     chpl examples/hello3-datapar.chpl

     # run the sample program
     ./hello3-datapar

  If you run into any portability issues, please see
  :ref:`readme-bugs`.

.. _using-chapel-in-another-shell:

Using Chapel in a Different Shell or Terminal
---------------------------------------------

Note that in both the Quickstart and preferred modes above, any
environment settings made by ``setchplenv.bash`` will not persist
beyond your current shell/terminal session.  One easy way to use
Chapel from a different shell or terminal is to re-``source`` the
``setchplenv.bash`` script that you used when building Chapel.
However since this can quickly become annoying, other strategies are
available including a ``./configure`` + ``make install`` option.  See
:ref:`using-chapel-in-other-sessions` for details.


Using Chapel in Multi-Locale Mode
---------------------------------

All of the instructions above describe how to run Chapel programs in a
single-locale (shared-memory) mode. To run using multiple locales
(multiple compute nodes with distributed memory), please refer to
:ref:`readme-multilocale`.


Notes on Performance
--------------------

If you plan to do performance studies of Chapel programs, be sure to
use the full-featured version above, to compile with ``--fast`` once
your program is correct, and to refer to
https://chapel-lang.org/perf-tips.html for other tips.


.. _quickstart-with-other-shells:

Quickstart with Other Shells
----------------------------

Use the table below to identify the location of an appropriate
Quickstart ``setchplenv`` script, based on the shell you use.

==================================== ==========================================
**If you use:**                       **then type:**
------------------------------------ ------------------------------------------
a bash-compatible shell (bash/zsh)   ``source util/quickstart/setchplenv.bash``
a csh-compatible shell (csh/tcsh)    ``source util/quickstart/setchplenv.csh``
the fish shell (fish)                ``. util/quickstart/setchplenv.fish``
the Bourne shell (sh)                ``. util/quickstart/setchplenv.sh``
==================================== ==========================================

Scripts that set the preferred environment for each shell can be
located by removing ``quickstart/`` from the paths above.


What's next?
------------

For further information about Chapel, refer to the following resources:

============================ ==================================================
Online documentation:        :ref:`chapel-lang.org/docs <chapel-documentation>`
Platform-specific notes:     :ref:`../platforms <platforms-index>`
Example Chapel programs:     ``$CHPL_HOME/examples/README``
Setting Chapel env vars:     :ref:`chplenv.rst <readme-chplenv>`
Building the compiler:       :ref:`building.rst <readme-building>`
Compiling Chapel programs:   :ref:`compiling.rst <readme-compiling>`
Executing Chapel programs:   :ref:`executing.rst <readme-executing>`
Debugging Chapel programs:   :ref:`debugging.rst <readme-debugging>`
Reporting bugs:              :ref:`bugs.rst <readme-bugs>`
Performance:                 https://chapel-lang.org/performance.html
Chapel modes for emacs/vim:  ``$CHPL_HOME/highlight/README.md``
Chapel's file/dir structure: ``$CHPL_HOME/README.files``
Changes since last release:  ``$CHPL_HOME/CHANGES.md``
============================ ==================================================
