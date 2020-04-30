.. _readme-globalvars:

==================================
Global Variables
==================================

.. warning:: The variables defined here are not part of the Chapel Standard and 
             they can be changed, renamed, or removed at any point in the future.

The Chapel Compiler defines some variables that are available to all Chapel programs. 
These variables are used by the compiler itself and the standard Chapel libraries/packages 
to provide some features/optimizations based on compilation flags. 
All of these variables are compile-time constants.

Variables
---------

**boundsChecking**

    This variable is *true* when the ``--bounds-checks`` flag is enabled. 
    It is used by libraries to enable/disable runtime bounds checks while 
    accessing ranges, arrays, or domains.

**castChecking**

    This variable reflects whether the ``--cast-checks`` flag is enabled. If this flag is enabled, checks will
    be done to prevent narrowing conversions.

**chpl_checkNilDereferences**

    When ``--nil-checks`` is enabled, this variable is set to *true*. It is used to check if an object
    is nil before accessing it.

**chpl_checkDivByZero**

    This variable can be used to check if ``--div-by-zero-checks`` is enabled. When this variable is
    set to *true*, the runtime will check for attempts to divide by zero.

**chpl_warnUnstable**

    This variable can be used to check if the ``--warn-unstable`` compiler flag is enabled. When set to *true*,
    the compiler will issue a warning about code that can be changed in the future or is not suitable for use.

**_local**

    This variable can be used to check if a program is being compiled for single or multi locale execution.
    It will be set to *true* when the program will be compiled for single locale execution.