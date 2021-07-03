.. _readme-buildingdocs:

=============================
Building Chapel Documentation
=============================

The easiest way to build the Chapel web documentation is to use the top level
make target::

   [g]make docs

This will build the documentation for the standard and internal modules using
chpldoc, and symlink all of the ``$CHPL_HOME/doc/`` documents over to
a sphinx project that lives in ``$CHPL_HOME/doc/``. Once built,
the documentation can be viewed at ``$CHPL_HOME/doc/html/index.html``.

Recent and previous releases of the documentation can be found at
https://chapel-lang.org/docs/.


=========================
How the HTML is Generated
=========================

The Chapel documentation shown at https://chapel-lang.org/docs/ is generated
in three different ways:

#.  Restructure Text files (with a .rst extension) are converted into .html
    files via sphinx (https://sublime-and-sphinx-guide.readthedocs.io/).
#.  Comments and code from Chapel source files are converted into .rst files
    using chpl2rst (FIXME: link to that).
#.  Some comments and code from Chapel source files are converted into .rst
    files using chpldoc (FIXME: link to that), which will leave out TODOs,
    FIXMEs, and other parts of the comments.


============================================
Organization of Chapel Documentation Sources
============================================

The file doc/rst/index.rst is the root file that sets up the
structure of the documentation and results in the current sidebar
displayed on https://chapel-lang.org/docs/.


strong correlation in side bar of https://chapel-lang.org/docs/ and rst/ subdirs, except lots of the stuff under Writing Chapel Programs

Looking at the sidebar of https://chapel-lang.org/docs/, we see:

--- the following all tend to just use sphinx

.. list-table:: Title
   :widths: 25 25 50
   :header-rows: 1

   * - Heading row 1, column 1
     - Heading row 1, column 2
     - Heading row 1, column 3
   * - Row 1, column 1
     -
     - Row 1, column 3
   * - Row 2, column 1
     - Row 2, column 2
     - Row 2, column 3

COMPILING AND RUNNING CHAPEL

Quickstart Instructions -> doc/rst/usingchapel/QUICKSTART.rst
Using Chapel -> doc/rst/usingchapel/*  # rst->html via sphinx
Platform-Specific Notes -> doc/rst/platforms/*  # "
Technical Notes -> doc/rst/technotes/*  # "
Tools -> doc/rst/tools/*   # "


WRITING CHAPEL PROGRAMS

Quick Reference -> language/reference.rst  # sphinx
--- these use chpl2rst.py on .chpl files and then sphinx to generate html
Hello World Variants -> examples/index.rst # "
                     -> + test/release/examples/hello*.chpl  # ./util/chpl2rst.py + sphinx
Primers -> primers/index.rst # sphinx
        -> + test/release/examples/primers/*.chpl   # chpl2rst.py + sphinx
--- sphinx again
        Language Specification -> language/spec/*   # sphinx

--- chpldoc + sphinx
Built-in Types and Functions -> doc/rst/builtins  # sphinx
                             -> modules/internal/*.chpl  # chpldoc + sphinx
Standard Modules -> doc/rst/modules/standard  # sphinx
                 -> modules/standard/*.chpl  # chpldoc + sphinx
Package Modules -> doc/rst/modules/packages  # sphinx
                -> modules/packages/*.chpl   # chpldoc + sphinx
Standard Layouts and Distributions -> doc/rst/modules/layoutdist  # sphinx
                                   -> modules/layouts/*  # chpldoc + sphinx
                                   -> modules/dists/*    # "

-- sphinx
Mason Packages  -> doc/rst/mason-packages.rst   # sphinx
Chapel Users Guide (WIP)  -> doc/rst/users-guide  # sphinx


LANGUAGE HISTORY

Chapel Evolution  -> doc/rst/language/evolution.rst  # sphinx
Documentation Archives -> language/archivedSpecs.rst  # sphinx

===============================================
Files that have information about documentation
===============================================

 - **chapel/README.devel**, how doc/ differs in the release vs. the dev version
 - **bestPractices/releasingDocs.md**, how to create the release documentation
   and push it to the website
 - **chapel/doc/README.rst**, talks about .rst and has a list of the documentation
   available in .rst
 - **chapel/doc/rst/developer/bestPractices/buildingdocs.rst**, this file
doc/
