.. _chapelhome-quickstart:

Chapel Quickstart Instructions
==============================

These instructions are designed to help users get started with a
source distribution of Chapel as quickly as possible.  If you are
interested in taking the time to understand Chapel's configuration
options, build process, and installation more thoroughly, please refer
to :ref:`readme-chplenv` and :ref:`readme-building` instead.

These instructions first have you build a minimal, stripped-down
version of Chapel to reduce build times and the potential for
confusion.  Once you are interested in a full-featured version of
Chapel, refer to :ref:`using-a-more-full-featured-chapel` below.


0) See :ref:`readme-prereqs` for information about system tools and
   packages you should have available to build and run Chapel.


1) If you don't already have the Chapel 1.25 source release, see
   https://chapel-lang.org/download.html.


2) Build Chapel in its 'Quickstart' configuration:

   a. Unpack the source release if you haven't already:

      .. code-block:: bash

         tar xzf chapel-1.25.0.tar.gz

   b. Make sure that you are in the directory that was created when
      unpacking the source release, for example:

      .. code-block:: bash

         cd chapel-1.25.0

   c. Set up your environment for Chapel's Quickstart mode.
      If you are using a shell other than bash,
      see :ref:`quickstart-with-other-shells` below.

      .. code-block:: bash

         source util/quickstart/setchplenv.bash

   d. Use GNU make to build Chapel.  On some systems, you may have to
      use ``gmake`` if ``make`` is not a GNU version.

      .. code-block:: bash

         make


3) Compile an example program:

   .. code-block:: bash

      chpl examples/hello3-datapar.chpl


4) Run the resulting executable:

   .. code-block:: bash

      ./hello3-datapar


5) Experiment with Chapel in this Quickstart mode to your heart's
   content.  Once you are comfortable with Chapel and interested in
   using a full-featured version in the preferred configuration, see
   the next section.


.. _using-a-more-full-featured-chapel:

Using Chapel in its Preferred Configuration
-------------------------------------------

To use Chapel in its preferred, full-featured mode, you will need to
rebuild Chapel from source in a different configuration:

*  Open up a new shell to avoid inheriting the previous environment
   settings.

* The Quickstart configuration described above sets ``CHPL_LLVM=none``
  for simplicity and to save time.  This causes the Chapel compiler to
  use its C back-end, which is not the preferred option.  As of Chapel
  1.25, LLVM is the default back-end, which needs to be available for
  full functionality.  There are a few ways to use LLVM:

  - ensure that you have a version of LLVM 11 installed on your system
    and set ``CHPL_LLVM=system`` (or leave it unset and Chapel should
    find it if it's in your path)

  - set ``CHPL_LLVM=bundled`` to have Chapel build and use the bundled
    version of LLVM (note that this can take a *long* time)

  - set ``CHPL_LLVM=none`` to continue using the C back-end rather
    than LLVM

*  Repeat steps 2-5 above, but in step 2, source ``util/setchplenv.bash``
   instead of ``util/quickstart/setchplenv.bash``.
   This will set up your environment to use Chapel in the preferred
   configuration.  Note that building this configuration involves
   compiling third-party packages, which will increase the overall
   build time.

   .. code-block:: bash

      # Set environment variables to preferred configuration
      source util/setchplenv.bash

      # re-build Chapel
      make

      # compile a sample program
      chpl -examples/hello3-datapar.chpl

      # run the sample program
      ./hello3-datapar

   If you run into any portability issues, please see
   :ref:`readme-bugs`:


Using Chapel in a Different Shell / Session
-------------------------------------------

Note that in both the Quickstart and preferred modes above, any
environment settings made by ``setchplenv.bash`` will not persist
beyond your current shell / terminal session.  To use Chapel from a
different shell or terminal, you will need to either re-``source`` the
``setchplenv.bash`` script that you used when building Chapel or set
up your environment to support additional shells automatically.  See
:ref:`using-chapel-in-other-sessions` for details.


Using Chapel in Multi-Locale Mode
---------------------------------

All of the instructions above describe how to run Chapel programs in a
single-locale (shared-memory) mode. To run using multiple locales
(distributed memory), please refer to :ref:`readme-multilocale`.


Notes on Performance
--------------------

If you plan to do performance studies of Chapel programs, be sure to
use the full-featured version above and see
https://chapel-lang.org/perf-tips.html for valuable tips.


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

For the full-featured builds using these shells, simply remove
``quickstart/`` from the path.


What's next?
------------

For more information about Chapel, refer to the following resources:

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
