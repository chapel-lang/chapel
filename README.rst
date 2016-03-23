.. image:: http://chapel.cray.com/images/cray-chapel-logo-200.png
    :align: center


.. _chapelhome-readme:

The Chapel Language
===================

Chapel is an emerging programming language designed for productive parallel
computing at scale. Chapel's design and implementation have been undertaken
with portability in mind, permitting Chapel to run on multicore desktops and
laptops, commodity clusters, and the cloud, in addition to the high-end
supercomputers for which it was designed. Chapel's design and development are
being led by `Cray Inc.`_ in collaboration with academia, computing centers,
and industry. See `chapel.cray.com`_ for more information.

.. _Cray Inc.: http://www.cray.com/
.. _chapel.cray.com: http://chapel.cray.com/

This is the 1.13.0 release of the Chapel compiler, intended to give
potential users a look at what we're doing and the opportunity to
provide us with feedback.  See the LICENSE file for the release's
licensing terms.

.. _chapelhome-quickstart:

Quick Start Instructions
------------------------

The following instructions are designed to get you up and running with
Chapel with a minimum of fuss.  Note that building and using Chapel in
this mode disables enhanced runtime options and optional language
capabilities in the interest of a simple and clean build.




0) See `prereqs.rst`_ for more information about packages you
   might need to have installed for Chapel to work. Download
   a Chapel source release from `download.html`_ if you haven't already.

.. _prereqs.rst: http://chapel.cray.com/docs/1.13/usingchapel/prereqs.html
.. _download.html: http://chapel.cray.com/download.html



1) Make sure that your shell is in the directory containing this
   ``README.rst`` file.  For example:

    .. code-block:: sh

        cd ~/chapel-1.13.0



2) Set up your environment to use Chapel in "Quick Start" mode.

   You must be in the top-level Chapel directory for the command
   scripts below to work properly.  Note that there is no requirement to use
   these scripts long-term, they are merely designed to get new users
   up and running quickly.  See `chplenv.rst`_ for a complete
   description of Chapel's environment variables and their options.

   If you use a shell other than the ones listed below and are able to
   create an equivalent ``util/quickstart/setchplenv.*`` script for it,
   please consider contributing a copy of it back to the project for
   inclusion in the next release.

.. _chplenv.rst: http://chapel.cray.com/docs/1.13/usingchapel/chplenv.html

======================================== ==========================================
**If you use:**                           **then type:**
---------------------------------------- ------------------------------------------
a bash-compatible shell (bash/zsh)       ``source util/quickstart/setchplenv.bash``
a csh-compatible shell (csh/tcsh)        ``source util/quickstart/setchplenv.csh``
the fish shell (fish)                    ``. util/quickstart/setchplenv.fish``
the Bourne shell (sh)                    ``. util/quickstart/setchplenv.sh``
======================================== ==========================================



3) Build the compiler and runtime libraries using:

        ``gmake``

   or if your default make is GNU make compatible (as on Mac OS X or
   Cygwin), use:

        ``make``

   Parallel build execution (e.g. ``gmake -j``) is supported.



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



8) Experiment with Chapel in Quick Start mode to your heart's content.

   Once you are comfortable with Chapel and interested in using a
   full-featured version in the preferred configuration:

   a) Open up a new shell to avoid inheriting the previous environment
      settings.

   b) Repeat steps 1-7 above, but in Step 2, use ``util/setchplenv.*``
      instead of ``util/quickstart/setchplenv.*``

   This will set up your environment to use Chapel in the preferred
   configuration.  Making this configuration will involve building one
   or more third-party packages which will increase the overall build
   time.  If you run into any portability issues, please let us know
   at chapel_info@cray.com.



9) If you plan to do performance studies of Chapel programs, be sure
   to (a) use the full-featured version and (b) read the ``PERFORMANCE``
   file in this directory to avoid common pitfalls.



10) All of the instructions above describe how to run Chapel programs
    in a single-locale / shared-memory mode. To run on multiple compute
    nodes, please refer to `multilocale.rst`_.

.. _multilocale.rst: http://chapel.cray.com/docs/1.13/usingchapel/multilocale.html




What's next?
------------
=============================================== =====================================
**For more detailed information about:**        **refer to:**
----------------------------------------------- -------------------------------------
    changes since the last release              ``CHANGES``
    implementation status                       ``STATUS``
    performance status                          ``PERFORMANCE``
    Chapel's file and directory structure       ``README.files``
    prerequisites for using this release        `prereqs.rst`_
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
