.. _readme-voidVariables:

=========================
Void Variables and Fields
=========================

This README describes support for ``void`` variables and class/record
fields.  A variable or field with type ``void`` will be removed from
the program by the compiler.  It cannot be assigned to a variable of
any non-void type, and non-void values cannot be assigned to a ``void``
variable.  A ``void`` variable cannot be used in any context that
requires a non-void value.

The value with type ``void`` is currently named ``_void``. It is
expected that this name will be changed.

A ``void`` value can be used to instantiate fully-generic function
arguments as long as the argument is not used in any non-void context.
This will cause the function argument to be removed by the compiler.

How is this useful?
-------------------

Often a class or record will have different implementation strategies
depending on things like the properties of the hardware or the type of
a generic field.  When the different implementation strategies each
require their own class/record fields, the number of fields in the type
grows, ballooning the size of the type with unused variables.

The type ``void`` was introduced to allow the unused variables to be
removed by the compiler. For example, if some fields are used only
when the param ``isAccelerator`` is ``true``, they can be declared
with a conditional type that is ``void`` when it is ``false``. Such
fields will be removed by the compiler.

.. code-block:: chapel

  record R {
    param isAccelerator: bool;
    var accelVar: if isAccelerator then real else void;

    proc doComputation() {
      if isAccelerator {
        ... accelVar ...
      } else {
        // non-accelerator version
      }
    }
  }

Void Functions and Iterators
----------------------------

A function may return the ``void`` value ``_void``.  Functions that
return the value ``_void`` have a return type of ``void`` and can be
used as ``void`` values.

A function that does not return also has a return type of ``void``,
but can not be used as a ``void`` value. For example, it cannot be
assigned to a void-variable.

An iterator may yield the ``void`` value ``_void``.  A loop calling
the iterator will iterate once per ``_void`` yielded with a ``void``
index variable.

An iterator with no ``yield`` has type ``void``. A loop calling the
iterator will execute the body of the iterator, but no loop iterations.

.. code-block:: chapel

  proc noReturn(): void {
    writeln("Hello from noReturn");
  }

  //var v1: void = noReturn();
  //var v2 = noReturn();

  proc explicitReturn(): void {
    writeln("Hello from explicitReturn");
    return _void;
  }

  var v3: void = explicitReturn();
  var v4 = explicitReturn();

  iter noYield() {
    writeln("noYield iter");
  }

  iter yieldVoids() {
    for i in 1..2 do
      yield _void;
  }

  for i in noYield() {
    writeln("Unprinted");
  }

  for i in yieldVoids() {
    writeln("Printed twice");
  }

Although the function ``noReturn()`` has a return type of ``void``,
it cannot be assigned to a variable. Both the ``v1`` and ``v2``
assignments above would be errors if uncommented. A function that returns
a ``void`` value explicitly is assignable to a variable of type ``void``,
as in the ``v3`` and ``v4`` examples.

The ``for`` loop calling the ``noYield()`` iterator will execute the
iterator body once, printing "noYield iter", but will not execute the
loop body.  The loop calling the ``yieldVoids()`` iterator will execute
the loop body twice because the iterator yields two ``_void`` values.
