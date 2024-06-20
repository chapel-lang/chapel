.. _readme-remote:

.. default-domain:: chpl

Remote Variable Declarations
============================

.. warning::

  This work is under active development. As such, the interface and behavior
  of tremote variables is unstable and expected to change.

.. contents::

Introduction
------------

Remote variable declarations make it possible to store a particular variable
on locale different from the one currently executing the code. Unlike plain
variable declarations in tandem with the ``on`` statement, remote variables
do not introduce a new scope. Specifically, consider the following program:

.. code-block:: chapel

   on someOtherLocale {
     var myVar = 42;
   }
   writeln(myVar); // Error: myVar is not in scope

In the above program an ``on`` block is used to declare a variable on a different
locale. However, because the ``on`` block creates a new scope, this variable
is not visible to the ``writeln`` statement. The most straightforward way
to return execution to the original locale is to use a second, nested ``on``
block:

.. code-block:: chapel

   on someOtherLocale {
     var myVar = 42;
     on originalLocale {
       writeln(myVar);
     }
   }

Remote variable declarations avoid this problem by allowing variables on
different locales to be declared without creating a new scope.

.. code-block:: chapel

   on someOtherLocale var myVar = 42;
   writeln(myVar); // Output: 42

Syntax and Semantics
--------------------

Remote variable declarations are created by prefixing a regular variable
declaration with ``on`` followed by the target locale. Declarations with
only a type, only an initializer, or both are supported:

.. code-block:: chapel

   on loc var myVar1 = 42;
   on loc var myVar2: 42;
   on loc var myVar3: int = 42;


Remote variables are stored in the heap memory of the target locales.
Currently, only ``var`` and ``const`` declarations are allowed. The semantics
of the type-only form are the same as for the type-only form of regular
variable declarations: the type's default value is used.

The intended semantics of the remote variable declaration is to act similarly
to two nested ``on`` blocks, with the exception of the scope changes. Thus,
the following code:

.. code-block:: chapel

   on loc var myVar = 42;
   doSomething(myVar);

Should behave roughly the same as:

.. code-block:: chapel

   on loc {
     var myVar = 42;
     on originalLocale {
       doSomething(myVar);
     }
   }

.. warning::

  The current implementation does not always execute the initialization
  expression on the target locale; as a result, today, the two pieces
  of code above are not equivalent. This is considered a bug and will
  be fixed in a future release.

Array types and promotion are supported, making the following code valid:

.. code-block:: chapel

   on loc var A: [1..10] int;
   on loc var B = A + 1;

A useful pattern with Chapel's `GPU Support <./gpu.html>`_ is to declare an array
on the GPU using remote variable declarations, and copy a portion
of a host array into it:

.. code-block:: chapel

   on here.gpus[0] var GpuA = A[GpuPortion];

If the target locale is a GPU locale, and the initialization expression
is either a promoted expression or a loop expression, it is transformed into
a GPU kernel as it would be in an ``on`` block. Thus, the following line of
code generates a kernel launch:

.. code-block:: chapel

   on here.gpus[0] var B = foreach i in 1..128 do i*i;

As with ``on`` blocks, only GPU-eligible loops are transformed into GPU
kernels. To assert that the initializer of a GPU-bound remote variable is
transformed into a GPU kernel, you can use the ``@assertOnGpu`` attribute:

.. code-block:: chapel

   @assertOnGpu
   on here.gpus[0] var B = foreach i in 1..128 do i*i;

.. _remote-variables-implementation:

Implementation Details
----------------------

As mentioned above, remote variables are declared in the heap memory of
the target locale. This is done by desugaring the remote variable
to the allocation of class-based Chapel wrapper. The following statement:


.. code-block:: chapel

   on loc var myVar = 42;

Is desugared to something like the following:

.. code-block:: chapel

  class _remoteVarContainer {
    var containedValue;
  }

  record _remoteVarWrapper {
    type eltType;
    var tmp: owned _remoteVarContainer(eltType);

    proc ref get() ref {
      return tmp.containedValue;
    }
  }

  var wrapped_myVar = chpl__buildRemoteVarWrapper(loc, 42);
  ref myVar = wrapped_myVar.get();

When building the remote wrapper, the ``_remoteVarContainer`` is allocated
on the heap in the target locale; since ``myVar`` becomes a reference to a
part of that container, it also lives on the target locale.


Limitations and Future Work
---------------------------

Remote variable declarations generally work as expected, and support a variety
of input types. However, there are still some limitations that are considered
future work:

* As mentioned above, today the initialization expression of a remote variable is
  executed on the current locale instead of the target locale. The only
  exception to this rule is when the initialization expression is an array
  promotion or a loop expression. This is inconsistent, and makes remote variables
  behave differently from the "two nested ``on`` blocks" pattern.
* The heap-based container for the remote variable is always created, even when
  the variable is itself a heap-allocated class. This introduces an additional
  level of indirection in certain cases.
* Unlike regular ``on`` statements, remote variable declarations require
  the destination to be a locale. Whereas ``on someVar {`` is used to
  move execution to the locale on which ``someVar`` is declared, to achieve
  the same effect with remote variable declarations, the locale must be
  queried: ``on someVar.locale myVar = 42;``.
* Because of the desugaring described in the :ref:`remote-variables-implementation`
  section, and because records and classes today don't support ``ref`` fields,
  remote variables cannot be used as fields of records or classes.
