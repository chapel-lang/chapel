===============================
Best Practices for Contributors
===============================

.. comment:

   update the following toctree when the document is linked in here,
   so that we can be sure everything is included.

.. toctree::
   :maxdepth: 1
   :hidden:

   GettingStarted
   ContributorInfo
   DCO
   CompilerDebugging
   CompilerIRTricks
   ErrorWarningMessaging
   RuntimeLibrary
   GeneratedCode
   GASNetOnDesktops
   git
   CHPL_DEVELOPER
   TestSystem
   SpellChecking
   Valgrind
   Sanitizers
   Deprecation
   Unstable
   NightlyTesting
   StandardModuleStyle
   TestAnnotationsLocally
   buildingdocs
   Potpourri

This directory contains rough notes intended to be helpful in pointing
out features/quirks of the Chapel development experience for new
developers.  A possible reading order is roughly as follows:

:ref:`best-practices-getting-started`:
  A list of suggested "first steps" for new developers.


:ref:`best-practices-contributor-info` and :ref:`best-practices-dco`:
  Information for contributors about setting up github accounts, the developer workflow, and testing changes.


`Compiler documentation`: 
  :ref:`compiler-internals-index`:
    API documentation for the frontend library

  The compiler overview document in 

  ``$CHPL_HOME/doc/rst/developer/implementation/compilerOverview``

  :ref:`best-practices-compiler-debugging`:
    Tips on debugging the compiler.

  :ref:`best-practices-compiler-ir-tricks`:
    Tips on examining compiler IR during/after compilation.

  :ref:`best-practices-error-warning-messaging`:
    The standard ways to report errors and warnings in ``Chapel/compiler/runtime``.

:ref:`best-practices-runtime-library`:
  All about the runtime library.

:ref:`best-practices-generated-code`:
  All about compiler-generated code: where it is, timing it, debugging/profiling, etc.

:ref:`best-practices-gasnet-on-desktops`:
  Tips on running and debugging Chapel programs with GASNet on your desktop.

:ref:`best-practices-git`:
  Tips for using git.

:ref:`best-practices-chpl-developer`:
  The most important environment variable you want to set, and what it does.

:ref:`readme-testsystem`:
  All about the Chapel testing system.

:ref:`best-practices-spell-checking`:
  How to use chplspell to spellcheck the Chapel project.

:ref:`best-practices-valgrind`:
  Building the Chapel compiler so that compiled programs work with valgrind.

:ref:`best-practices-sanitizers`:
  Enabling sanitizers for the compiler and/or executable.

:ref:`best-practices-deprecation`:
  How to deprecate standard library routines

:ref:`best-practices-unstable`:
  How to mark features as unstable

:ref:`readme-nightlytesting`
  How to run nightly testing

:ref:`best-practices-standard-module-style`:
  Style guidance when writing standard modules

:ref:`best-practices-test-annotations-locally`:
  How to test ANNOTATIONS.yaml changes locally

:ref:`readme-buildingdocs`:
  Information about building and updating the Chapel documentation

:ref:`best-practices-potpourri`:
  Notes not in any of the above categories.
