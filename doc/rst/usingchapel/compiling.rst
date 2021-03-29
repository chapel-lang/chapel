.. _readme-compiling:

=========================
Compiling Chapel Programs
=========================

The Chapel compiler converts programs expressed in Chapel source code
into the corresponding executable form.  This document briefly discusses
running the Chapel compiler.

.. contents::


------------
Getting Help
------------

If you have set up your environment as described in :ref:`readme-chplenv` and
built the compiler as described in :ref:`readme-building`, you can see the
Chapel compiler's help message using:

  .. code-block:: sh

     chpl --help

This displays the compiler's command-line flags sorted by category.
For each flag, the short one-letter form is given (if one exists),
followed by the long form of the flag.  Next, the pattern for any
arguments to the flag are given.  And finally a brief description of
the flag is provided.  A flag may have a corresponding environment
variable, setting which is equivalent to passing the flag.  These
variables are listed by the ``--help-env`` option.  The flags' current
settings are listed by ``--help-settings``.

More detailed information about the compiler and its command-line
flags is also available on its :any:`man page <man>`, which can be viewed from
the command-line using:

  .. code-block:: sh

     man chpl


-----------------
Most Useful Flags
-----------------

We note some of the most useful flags here:

=====================   ======================================================
Flags                   Description
---------------------   ------------------------------------------------------
``-o <filename>``       specify the filename of the generated executable,
                        otherwise the name of the file containing the main
                        module is used (minus its ``.chpl`` extension).
``--no-checks``         turns off runtime semantic checks like bounds
                        checking and nil class instance dereferencing
``-O``                  turns on optimization of the generated C code
``--fast``              turns on ``--no-checks``, ``-O``, and enables
                        many other optimizations
``--savec <dir>``       saves the generated C code in the specified
                        directory
``-g``                  support debugging of the generated C code
``--ccflags <flags>``   specify flags that should be used when invoking
                        the back-end C compiler
``--ldflags <flags>``   specify flags that should be used when invoking
                        the back-end linker
``-s <name[=expr]>``    set a config variable with the given expression
                        as its default value (config params must be set
                        to values that are known at compile time)
``--print-passes``      print the compiler passes as they execute
``--print-commands``    print the system commands that the compiler
                        executes
``--print-code-size``   prints some code size statistics about the
                        number of lexical tokens per line, as well as
                        the number of code, comment, and blank lines
``--version``           print the Chapel compiler version number
``--help``              print a brief overview of the command-line
                        options
``--help-env``          lists the environment variables for each
                        command-line flag
``--help-settings``     lists the current setting of each command-line
                        flag
=====================   ======================================================


------------------------
Tab Completion for Flags
------------------------

Bash users can source the script ``$CHPL_HOME/util/chpl-completion.bash`` to
enable tab-completion for chpl options.  After sourcing the
chpl-completion.bash script tab completion can be used:

  .. code-block:: sh

     % chpl --ca<tab><tab>

Will print the options that start with "--ca".

  .. code-block:: sh

     --cache-remote --cast-checks

Adding one more letter to differentiate and pressing tab again will
auto-complete the option and add a space, ready for the next option.

  .. code-block:: sh

     % chpl --cac<tab>
     % chpl --cache-remote 

If Z shell users want tab-autocompletion they need to export or add the 
$CHPL_HOME/util/_chapel-zsh-completion to their FPATH variable.
They will also need to add the following into their ~/.zshrc file.

  .. code-block:: sh

     autoload -U compinit
     compinit
