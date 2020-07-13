=====================
Chapel Best Practices
=====================

This directory contains rough notes intended to be helpful in pointing
out features/quirks of the Chapel development experience for new
developers.  A possible reading order is roughly as follows:

`GettingStarted.rst`: 
  A list of suggested "first steps" for new developers.

`ContributorInfo.rst`: 
  Information for contributors about setting up github accounts, the developer workflow, and testing changes.

`Compiler documentation`: 
  The compiler overview document in 

  ``$CHPL_HOME/doc/rst/developer/compilerOverview``

  `CompilerDebugging.rst`: 
    Tips on debugging the compiler.

  `CompilerIRTricks.rst`: 
    Tips on examining compiler IR during/after compilation.

  `ErrorWarningMessaging.rst`: 
    The standard ways to report errors and warnings in ``Chapel/compiler/runtime``.

`RuntimeLibrary.rst`: 
  All about the runtime library.

`GeneratedCode.rst`: 
  All about compiler-generated code: where it is, timing it, debugging/profiling, etc.

`GASNetOnDesktops.rst`: 
  Tips on running and debugging Chapel programs with GASNet on your desktop.

`GitCheatsheet.rst`: 
  Tips for using git.

`CHPL_DEVELOPER.rst`: 
  The most important environment variable you want to set, and what it does.

`TestSystem.rst`: 
  All about the Chapel testing system.

`SpellChecking.rst`: 
  How to use chplspell to spellcheck the Chapel project.

`Potpourri.rst`: 
  Notes not in any of the above categories.

`Valgrind.rst`: 
  Building the Chapel compiler so that compiled programs work with valgrind.

`Sanitizers.rst`:
  Enabling sanitizers for the compiler and/or executable.
