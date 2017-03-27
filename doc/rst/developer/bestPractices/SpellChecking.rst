=========
chplspell
=========

Overview
========

``util/devel/chplspell`` is a script to assist in spell-checking the
Chapel documentation and source code.  It is a wrapper around the
`scspell`_ source-code spell-checker.

.. _`scspell`: https://pypi.python.org/pypi/scspell3k


``chplspell`` provides four main conveniences over simply using ``scspell``:

1. It has built-in knowledge of which files and directories in the
   Chapel repository benefit from being spell-checked.
2. It directs ``scspell`` to use the project dictionary file
   ``$CHPL_HOME/util/devel/chplspell-dictionary``.
3. It recurses through directories given on the command line.
4. It invokes the ``scspell`` that's installed in the Chapel virtualenv.

This document describes some basic information about ``chplspell``, common
use cases, a description of the dictionary files, and some less common
use cases related to management of the dictionary files.


Basic information
-----------------

``chplspell`` depends on ``scspell`` being installed in the virtualenv.  To
install it, use

  .. code-block:: text

     make chplspell-venv


``scspell`` (and thus ``chplspell``) has two main modes of invocation:
interactive and non-interactive.  ``chplspell`` further provides two ways
of using each mode: spell-checking the whole project or only specific
files or directories.

``chplspell`` maintains a project dictionary for Chapel in
``$CHPL_HOME/util/devel/chplspell-dictionary``.  This dictionary file
contains several types of word lists, as supported by ``scspell``:

:Natural language dictionary:
  Words that may be found in any file.
:Programming-language dictionaries:
  Words that may be found in certain types of files, identified
  by file extension.
:File-specific dictionaries:
  Words that may be found only in particular files.


``chplspell`` passes nearly all command line options through to
``scspell``, and reports ``scspell``'s usage message when invoked with
``--help``.  See `the scspell page on python.org`__ for information
about ``scspell``'s command line arguments, approach to spell
checking source code, and user interface.

__ scspell_


``chplspell`` adjusts the command line in several ways:

1. ``chplspell`` passes options to ``scspell`` directing it at
   ``$CHPL_HOME/util/devel/chplspell-dictionary``.
2. If no files or directories are given on the command line, ``chplspell``
   invokes ``scspell`` on a default set of files and directories that make
   sense for the Chapel repository.
3. If directories are given on the command line, ``chplspell`` invokes
   ``scspell`` on all files of certain types within those
   directories, recursively.
4. In case 2 or 3 above, ``chplspell`` may invoke ``scspell`` twice: once for
   most of the files it finds, and again for any LaTeX files it finds.
   This is because LaTeX does not use C-style escapes.

   * This is a minor point, relevant only to understanding why, when
     you hit ^C, ``chplspell`` keeps spell-checking; and why words ignored
     with the "Ignore all" interactive command are forgotten when the
     LaTeX portion begins.


The configuration of the "default set of files and directories" is at
the top of the ``chplspell`` script, and may be easily altered.


Non-interactive invocation
==========================

The simplest use is to produce a non-interactive report for the
default files and directories of the Chapel repository.  As of the
date of this writing, there were still some words yet to be added to
the project dictionary or corrected, which make a good example:

  .. code-block:: text

     $ chplspell --report-only

     doc/rst/developer/bestPractices/README:50: 'chplspell' not found in dictionary (from token 'chplspell')
     CHANGES.md:445: 'chpldocumentation' not found in dictionary (from token 'chpldocumentation')
     CHANGES.md:2001: 'pshm' not found in dictionary (from token 'pshm')
     CHANGES.md:2341: 'pshm' not found in dictionary (from token 'pshm')
     CHANGES.md:3360: 'circularities' not found in dictionary (from token 'circularities')
     spec/Domains.tex:367: 'parsafe' not found in dictionary (from token 'parsafe')


``chplspell`` may also be invoked on only particular files or directories
named on the command line.  For example, this file with one tyop:

  .. code-block:: text

     $ chplspell --report-only doc/rst/developer/bestPractices/SpellChecking.rst
     doc/rst/developer/bestPractices/SpellChecking.rst:109: 'tyop' not found in dictionary (from token 'tyop')
     $

(The project dictionary now includes the word "tyop" for this file, so
the above command no longer produces that result.)


Interactive invocation
======================

``scspell`` provides an interactive mode for making corrections and for
adding words to the various dictionaries.  This mode is also available
through ``chplspell``.

See `the scspell page on python.org`__ for details.

``chplspell``'s invocation of ``scspell`` makes any requested
dictionary changes to ``$CHPL_HOME/util/devel/chplspell-dictionary``

__ scspell_



Dictionary file details
=======================

This section provides a few details about the format of ``scspell``'s
dictionary file.  Understanding of these details is not necessary to
make use of chplspell as described above.  It will be helpful in
making use of the more advanced options in the next section.


The natural language word list contains the words that may appear in
any file being spell checked.  It is the last word list in the
dictionary file, under the heading ``NATURAL:``.


A "programming language" word list is used in addition to the natural
language word list when the file being checked matches one of the file
extensions given for that word list.  They appear in the dictionary
file on lines beginning with ``FILETYPE:``, e.g.,

  .. code-block:: text

     FILETYPE: TeX/LaTeX; .tex, .bib


A file-specific word list is used in addition when a file has a
matching "file id".  These are stored in the dictionary file under
``FILEID:`` headings, e.g.

  .. code-block:: text

     FILEID: 42424242-4242-4242-4242-424242424242

There are two ways that a file id's association with a file may be
represented to ``scspell``:

1. The file contains the string "scspell-id: " followed by a file id;
   e.g., in a comment.
2. There is an entry in the "file id mapping file",
   ``$CHPL_HOME/util/devel/chplspell-dictionary.fileids.json``,
   associating the file name to the file id.  For example, the
   following file id is associated with three files in the Chapel
   repository:

.. code-block:: text

  "63b96a22-1e46-11e6-a3a6-10ddb1d4c3d5": [
    "doc/rst/developer/hdfs_and_chapel/API.tex",
    "doc/rst/developer/hdfs_and_chapel/examples.tex",
    "modules/packages/HDFSiterator.chpl"
  ],

If a file has a file id associated, when ``scspell`` offers to add an
unrecognized word to a dictionary, one of the offered dictionaries is
this ``(f)ile-specific dictionary``.

If there is no file id associated with the file, ``scspell`` will
instead offer the option to create a ``(N)ew file-specific
dictionary``.  This option will create the new file id, add it to the
``dictionary.fileids.json`` file, and add the unrecognized word to
that file-specific word list in the ``dictionary`` file.


If a file with a file-specific word list is moved or copied (e.g., the
shootout benchmarks), and the association is via the file id mapping,
``chplspell`` won't have the existing word list associated with the
new file.  The next section describes several ways to remedy this
situation and similar ones without creating duplicate file-specific word
lists.


As of this writing, no files in the Chapel repository contain a file
id literal; all file id mappings are done through the file id mapping
file.


Dictionary file management options
==================================

--rename-file
-------------

``chplspell`` makes ``scspell``'s --rename-file option available to
update the file id map after a file has been renamed:

  .. code-block:: text

     git mv path/to/old.chpl new/path/and/new.chpl
     chplspell --rename-file path/to/old.chpl new/path/and/new.chpl

Unfortunately there is not yet a straight-up ``--copy-file``


--merge-file-ids
----------------

``scspell`` also provides a --merge-file-ids option for the case that two
files have file-specific word lists, and the word lists are similar enough
that they should be merged.  The file ids may be given by the file id
literal string, or by the name of a file associated with the file id:

  .. code-block:: text

     chplspell --merge-file-ids one/file.chpl a/similar/file.chpl

The only impact of the order is which file id hex string ends up
associated with the files.

--delete-files
--------------

The ``--delete-files`` option to ``chplspell`` may be used to remove the
association between a file id and a deleted file from the dictionary
file.  If that was the only file associated with that file id,
``chplspell`` will also remove the file id itself and the file-specific
dictionary.

  .. code-block:: text

     git rm doc/obsolete doc/archaic.md
     chplspell --delete-files doc/obsolete doc/archaic.md


Edit the dictionary.fileids.json file
-------------------------------------

You can edit the file by hand to add a filename to a file id, or
change a filename.  The format is straightforward JSON.

One minor detail (likely of interest only to those so hung up on
minutiae as to write a spell checking utility) is that while ``scspell``
emits the file id mapping file with no trailing newline, most text
editors take some convincing to save a file that way.  To avoid git
commits fighting over that last byte, it'd be considerate to get rid
of that newline before committing.

``pico -L`` is the simplest way I've found.  Otherwise, you can make
the change, then invoke ``chplspell`` to get it to re-write the file.  The
file will be rewritten only if there are changes to make to it, so
you'll likely need to make two changes that add up to no effect, such
as the sequence

  .. code-block:: text

     chplspell --rename-file CONTRIBUTORS.md SCHMONTRIBUTORS.md
     chplspell --rename-file SCHMONTRIBUTORS.md CONTRIBUTORS.md

(No files are renamed by this -- these operations manipulate only the
file id mapping.)
