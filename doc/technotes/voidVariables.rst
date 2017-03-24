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

Void Functions
--------------

A function that does not return is treated as a function that returns a
``void`` value.  A non-returning function can be assigned to a variable
of type ``void``. Initializing a variable without a specified type with
a function that doesn't return will infer the variable type to be
``void``. The variables will be removed by the compiler.

A function may also explicitly return the ``void`` value ``_void``.
Functions that have no return and functions that return the value ``_void``
both have a return type of ``void``. Functions that have no return and
functions that return ``_void`` can both optionally specify their return
types as ``void``.

.. code-block:: chapel

  proc noReturn(): void {
    writeln("Hello from noReturn");
  }

  var v1: void = noReturn();
  var v2 = noReturn();

  proc explicitReturn(): void {
    writeln("Hello from explicitReturn");
    return _void;
  }

  var v3: void = explicitReturn();
  var v4 = explicitReturn();

In the future, a function that does not return may be differentiated from
a function that explicitly returns a ``void`` value. In that case, both
the ``v1`` and ``v2`` examples above would be errors, but a function that
returns a ``void`` value explicitly would still be assignable to a variable
of type ``void``, as is the case for ``v3`` and ``v4``.
