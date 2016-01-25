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
following command:

``brew install chapel``

  .. note::

     The Homebrew installation provides a minimal installation of Chapel for
     users to explore and test the language.
     Of the omitted features, :ref:`multilocale <readme-multilocale>` support
     is most notable.
     Users interested in utilizing all of the
     language's rich features should clone the repository_ or
     download a release_, and :ref:`build Chapel from source <readme-building>`.

.. _Homebrew: https://github.com/Homebrew/homebrew
.. _repository: https://github.com/chapel-lang/chapel
.. _release: https://github.com/chapel-lang/chapel/releases

-----
Xcode
-----

  .. warning::

    Be forewarned that we do not maintain the Xcode project file very regularly
    and appreciate updates to the project file from external contributors.

For Macintosh developers, there is an Xcode project file located in

``$CHPL_HOME/make/Chapel.xcodeproj/project.pbxproj``

that can be used to develop Chapel within Xcode by opening the project

``$CHPL_HOME/make/Chapel.xcodeproj``

