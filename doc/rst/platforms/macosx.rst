.. _readme-macosx:

========================
Using Chapel on Mac OS X
========================

By and large, Chapel can be used on a Macintosh running OS X just like
any other platform -- refer to the :ref:`chapelhome-quickstart` for more
information on building and using Chapel.

--------
Homebrew
--------

A minimal installation of Chapel can be obtained through Homebrew_ with the
following command::

    brew install chapel

The above command installs a release version of Chapel. It is also
possible to use Homebrew_ to build and install a development version
based on the master branch on GitHub::

    brew install chapel --HEAD

When using a Homebrew install of Chapel, the ``CHPL_HOME`` directory is
here::

    /usr/local/Cellar/chapel/<chapel-version>/libexec/

.. note::

   The Homebrew installation provides a minimal installation of Chapel for
   users to explore and test the language.
   Of the omitted features, :ref:`multilocale <readme-multilocale>` support
   is most notable.
   Users interested in utilizing all of the
   language's rich features should clone the repository_ or
   download a release_, and :ref:`build Chapel from source <readme-building>`.

.. _Homebrew: https://github.com/Homebrew/brew
.. _repository: https://github.com/chapel-lang/chapel
.. _release: https://github.com/chapel-lang/chapel/releases
