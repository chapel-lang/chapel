.. default-domain:: chpl

.. module:: Assert
   :synopsis: Support for simple assert() routines.

Assert
======
**Usage**

.. code-block:: chapel

   use Assert;


Support for simple assert() routines.

.. note:: All Chapel programs automatically ``use`` this module by default.
          An explicit ``use`` statement is not necessary.

In the current implementation, these asserts never become no-ops.  That is,
using them will always incur execution-time checks.


.. function:: proc assert(test: bool)

   
   Assert that a boolean condition is true.  If it is false, prints
   'assert failed' and halts the program.
   
   :arg test: the boolean condition
   :type test: `bool`

.. function:: proc assert(test: bool, args ...?numArgs)

   
   Assert that a boolean condition is true.  If it is false, prints
   'assert failed - ' followed by all subsequent arguments, as though
   printed using :proc:`~ChapelIO.write()`.
   
   :arg test: the boolean condition
   :type test: `bool`
   
   :arg args: other arguments to print

