.. _readme-buildingdocs:

===============================================
Files that have information about documentation
===============================================

 - **chapel/README.devel**, how doc/ differs in the release vs. the dev version
 - **bestPractices/releasingDocs.md**, how to create the release documentation
   and push it to the website
 - **chapel/doc/README.rst**, talks about .rst and has a list of the documentation
   available in .rst
 - **chapel/doc/rst/developer/bestPractices/buildingdocs.rst**, this file


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
    using chapel/doc/util/chpl2rst.py.
#.  Some comments and code from Chapel source files are converted into .rst
    files using chapel/tools/chpldoc/, which will leave out TODOs,
    FIXMEs, and other parts of the comments.


============================================
Organization of Chapel Documentation Sources
============================================

The file chapel/doc/rst/index.rst is the root file that sets up the
structure of the documentation and results in the current sidebar
displayed on https://chapel-lang.org/docs/.


There is a strong correlation in side bar of
https://chapel-lang.org/docs/ and the doc/rst/ subdirectories,
except for lots of the stuff under Writing Chapel Programs.

Looking at the sidebar of https://chapel-lang.org/docs/, we see ...

.. list-table:: Using Sphinx to convert .rst files to .html files
   :widths: 100 100 100
   :header-rows: 1

   * - Doc name in Sidebar
     - Source file(s)
     - Sidebar subheading
   * - Quickstart Instructions
     - doc/rst/usingchapel/QUICKSTART.rst
     - COMPILING AND RUNNING CHAPEL
   * - Using Chapel
     - doc/rst/usingchapel/*
     - COMPILING AND RUNNING CHAPEL
   * - Platform-Specific Notes
     - doc/rst/platforms/*
     - COMPILING AND RUNNING CHAPEL
   * - Technical Notes
     - doc/rst/technotes/*
     - COMPILING AND RUNNING CHAPEL
   * - Tools
     - doc/rst/tools/*
     - COMPILING AND RUNNING CHAPEL
   * - Quick Reference
     - language/reference.rst
     - WRITING CHAPEL PROGRAMS
   * - Mason Packages
     - doc/rst/mason-packages.rst
     - WRITING CHAPEL PROGRAMS
   * - Chapel Users Guide (WIP)
     - doc/rst/users-guide
     - WRITING CHAPEL PROGRAMS
   * - Chapel Evolution
     - doc/rst/language/evolution.rst
     - LANGUAGE HISTORY
   * - Documentation Archives
     - language/archivedSpecs.rst
     - LANGUAGE HISTORY


.. list-table:: Use chpl2rst.py on .chpl files and then sphinx to generate html
   :widths: 100 100 100
   :header-rows: 1

   * - Doc name in Sidebar
     - Source file(s)
     - Sidebar subheading
   * - Hello World Variants
     - examples/index.rst + test/release/examples/hello*.chpl
     - WRITING CHAPEL PROGRAMS
   * - Primers
     - primers/index.rst + test/release/examples/primers/\*.chpl
     - WRITING CHAPEL PROGRAMS



.. list-table:: Use chpldoc on .chpl files and then sphinx to generate html
   :widths: 100 100 100
   :header-rows: 1

   * - Built-in Types and Functions
     - doc/rst/builtins + modules/internal/\*.chpl
     - WRITING CHAPEL PROGRAMS
   * - Standard Modules
     - doc/rst/modules/standard + modules/standard/\*.chpl
     - WRITING CHAPEL PROGRAMS
   * - Package Modules
     - doc/rst/modules/packages + modules/packages/\*.chpl
     - WRITING CHAPEL PROGRAMS
   * - Standard Layouts and Distributions
     - doc/rst/modules/layoutdist + modules/layouts/\*.chpl + modules/dists/\*.chpl
     - WRITING CHAPEL PROGRAMS


===============================================
Files that have information about documentation
===============================================

 - **chapel/README.devel**, how doc/ differs in the release vs. the dev version
 - **bestPractices/releasingDocs.md**, how to create the release documentation
   and push it to the website
 - **chapel/doc/README.rst**, talks about .rst and has a list of the documentation
   available in .rst
 - **chapel/doc/rst/developer/bestPractices/buildingdocs.rst**, this file


