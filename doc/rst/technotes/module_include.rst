.. _readme-module_include:

Including Sub-Modules from Separate Files
=========================================

This document describes draft support for including a submodule from a
separate file. This support is new in 1.21 and is considered unstable -
it might change in future releases.

Example
+++++++

The basic idea is that a module ``M`` in a file ``M.chpl`` can break its
submodules across multiple files by placing them in a directory called
``M/``.

The following example shows both the directory layout and file contents:

::

  M.chpl
  M/
      L.chpl

  program.chpl # Uses M


``M.chpl`` contains the following:

.. code-block:: chapel

  module M {
    include module L; // finds module L in L.chpl
    // equivalent to including L.chpl here, e.g.
    // module L {  ... }
  }


``M/L.chpl`` contains the following:

.. code-block:: chapel

  module L {
    writeln("In module L");
  }

``program.chpl`` contains the following:

.. code-block:: chapel

  import M;
  // Could refer to public symbols in M.L

The program is compiled with this command:

::

  chpl program.chpl

Visibility Details
++++++++++++++++++

Module visibility can be specified at the ``include module`` statement,
as with ``include private module L;`` or ``include public module L``. As
with regular ``module { }`` statements, the default is public. In other
words - ``include module L;`` is equivalent to ``include public module
L``.

Module visibility can additionally be specified inside the file declaring
the submodule. For example, ``L.chpl`` could contain
``public module L { }`` or ``private module L { }``. If the module
declaration indicates ``private`` visibility but the ``include module``
line does not, the compiler will issue an error. Otherwise, the
visibility at the ``include module`` line will be used.


Errors
++++++

It is currently an error if ``include module`` is used within a module
that is inside an inline submodule in the file containing that code. Such
cases can be handled by using ``include module`` for all of the levels of
submodule in the file.

It is currently an error to if ``include module`` is used within a module
in a file that has a name other than the module name (plus .chpl)
