.. _readme-module_search:

Module Search Paths
===================

This document describes how modules are located by the current
implementation of Chapel.

Internal modules are always loaded by a chapel program *as if* the
statement ``use ChapelStandard;`` has been inserted ahead of the first
statement in the main module.  Unlike standard modules, internal modules cannot
be overridden.

The compiler uses three module search paths: one for internal modules, one for
standard and package modules, and one for user modules.  The user path first
includes all the paths used to specify module files directly on the ``chpl``
command line and then includes the paths specified via ``-M`` commands or
through the ``CHPL_MODULE_PATH`` environment variable.

The module search paths are initialized as follows:

    * Internal
        * various directories under ``$CHPL_HOME/modules/internal``

    * Standard
        * ``$CHPL_HOME/modules/standard/gen/$CHPL_TARGET_PLATFORM-$CHPL_TARGET_COMPILER``
        * ``$CHPL_HOME/modules/standard``
        * ``$CHPL_HOME/modules/packages``
        * ``$CHPL_HOME/modules/layouts``
        * ``$CHPL_HOME/modules/dists``
        * ``$CHPL_HOME/modules/dists/dims``
    * User
        * ``$CHPL_MODULE_PATH``
        * ``(User paths)``

When the root internal module *ChapelStandard* and its dependencies are
loaded, the internal module path is searched first.  If a module is found there,
it is marked as internal.  Otherwise, it is skipped.

The user's modules are parsed next.  If a module named in a use statement is not
found, the standard version is loaded instead if it exists.  Otherwise, a
file-not-found error will be issued.  If both user and standard versions of a
module exists, the user is warned that the standard version is being overridden.
