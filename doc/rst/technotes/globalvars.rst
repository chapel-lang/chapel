.. _readme-globalvars:

==================================
Global Variables
==================================

.. warning:: The variables defined here are not part of Chapel Standard and 
             they can be changed, renamed or removed at any point in future.

Chapel Compiler define some variables that are available to all the Chapel programs. 
These variables are used by compiler itself and standard Chapel libraries/packages 
to provide some features/optimizations on the basis of compiler options. 
All these variables are compile-time constants.

Variables
---------

**boundsChecking**

    This variable can be used to check if ``bounds-checks`` is enabled or not. 
    It is used by libraries to enable/disable runtime bound checks while 
    accessing ranges, arrays or domains.

**castChecking**

    This variable reflects wether ``cast-checks`` is enabled or not.
    It is used to provide checks before casting a variable from one type to another.

**chpl_checkNilDereferences**

    When ``nil-checks`` is enabled, this variable is set to *true*. It is used to check if a object
    is nil before accessing it.

**chpl_checkDivByZero**

    This variable can be used to check if ``div-by-zero-checks`` is enabled. When this variable is
    set to *true*, the runtime will check for attempt to divide by zero.

**chpl_warnUnstable**

    This variable can be used to check if, ``warn-unstable`` compiler flag is enabled. When set to *true*,
    compiler will issue warning on the use of code that can be changed in future or are not suitable for use.

**_local**

    This variable can be used to check if program is being compiled for singlw/multi locale excecution.
    It is set to *true* if program is compiled for single locale excecution.