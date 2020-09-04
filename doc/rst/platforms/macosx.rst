.. _readme-macosx:

========================
Using Chapel on Mac OS X
========================

There are two main approaches for using Chapel on Mac OS X:

1) Install via Homebrew_.  For Homebrew users, this is the
   quickest way to get up and running, but it results in a copy of
   Chapel that only supports shared-memory (single-locale) executions.

2) Install and build Chapel from source, as with any other UN*X
   system.  This is slightly more involved, but supports Chapel's full
   feature set.  Refer to the :ref:`chapelhome-quickstart` for more
   information.

--------
Homebrew
--------

A minimal installation of Chapel can be obtained through Homebrew_ with the
following commands::

    brew update
    brew install chapel

These commands install the latest release of Chapel.  When using a
Homebrew installation of Chapel, the ``CHPL_HOME`` directory is here::

    /usr/local/Cellar/chapel/<chapel-version>/libexec/

Compile and run a test program::

    chpl /usr/local/Cellar/chapel/<chapel-version>/libexec/hello.chpl
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
   install a development version based on the master branch on
   GitHub::

    brew install chapel --HEAD


.. _Homebrew: https://brew.sh/
.. _whatsnext: 
