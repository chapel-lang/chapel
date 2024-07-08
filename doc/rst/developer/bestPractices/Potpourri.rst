.. _best-practices-potpourri:

===================
Miscellaneous Notes
===================

README files
------------

The repository has a collection of README files on various topics. The
main file is ``$CHPL_HOME/README.rst``.  Each file points to other related
READMEs so that any README can be reached from the main one by
following such **"links"**.

Some directories also have ``README.devel``, which contain additional
information for Chapel developers. They are not (necessarily)
referenced in the above collection.

The ``non-.devel`` READMEs are written so they are relevant to **users** of
Chapel. Therefore they reflect the directory structure of Chapel's
public releases; see ``$CHPL_HOME/README.devel`` for details.


grep shortcuts
--------------

In ``$CHPL_HOME/util/devel/`` - grep these files:

* grepcomp    - compiler sources

* grepdocs    - READMEs in ``$CHPL_HOME`` (except ``test/`` and ``third-party/``)

* grepmake    - Makefiles

* grepmod     - Chapel module sources

* greprt      - runtime library sources

* grepchpl    - all of the above

* grepstdchdrs - grep C files to look for std C header ``#includes``

* greptests   - grep files in the test subtree.  Takes argument with ``-t`` to
  specify the file type to search through (e.g. ``-t tests`` to search ``.chpl``
  files, ``-t outputs`` to search ``.good`` and ``.bad`` files, etc), defaulting
  to ``tests``.
