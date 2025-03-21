.. _readme-throwing-initializers:

.. default-domain:: chpl

=====================
Throwing Initializers
=====================

Overview
--------

Initializers (see :ref:`Class_Initializers`) and :ref:`Chapter-Error_Handling`
are currently in the process of being integrated together.  Support for the
overlap of these features is currently limited, but key functionality has been
enabled.

Using Catch-less try!
~~~~~~~~~~~~~~~~~~~~~

``try!`` can be used anywhere in the body of any initializer, so long as it does
not have an associated ``catch`` clause.  When used, any error encountered will
cause the program to halt.

*Example (try-bang-init.chpl)*.

.. code-block:: chapel

   proc throwingFunc(x: int) throws {
     if (x > 10) {
       throw new Error("x too large");
     } else {
       return x;
     }
   }

   record Foo {
     var x: int;

     proc init(xVal: int) {
       x = try! throwingFunc(xVal); // try! here is legal
       /* The following code is not legal in an initializer yet, due to
          having a catch clause:

         try! {
           x = throwingFunc(xVal);
         } catch e: Error {
           x = 9;
         }
       */
     }
   }

.. BLOCK-test-chapelpost

   var f1 = new Foo(4);
   writeln(f1);
   var f2 = new Foo(11);

.. BLOCK-test-chapeloutput

   (x = 4)
   uncaught Error: x too large
     try-bang-init.chpl:3: thrown here
     try-bang-init.chpl:13: uncaught here

.. _init_declaring_init_as_throws:

Declaring throwing Initializers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Like typical routines, initializers and post-initializers
(``postinit()`` procedures) can be declared with the ``throws``
keyword.  This enables errors in the bodies of these procedures to be
thrown back to the calling context.  When an error is thrown, the
memory that would have been used for the result of the initializer
will be freed before the error is thrown back to the caller.

Note that, at present, this feature has two limitations:

* The fields of the object will not have their deinitializers
  (``deinit()`` procedures) called as they should be (see
  https://github.com/chapel-lang/chapel/issues/26437).

* Initializers can only throw errors via calls to throwing routines
  (i.e., they cannot contain ``throws`` statements directly) — and
  these calls must come after the ``init this`` statement (see
  :ref:`Limitations_on_Instance_Usage_in_Initializers` for information
  on ``init this`` and the example in
  :ref:`init_declaring_init_as_throws`).

*Example (init-declared-throws.chpl)*.

.. code-block:: chapel

   proc validate(r: R) throws {
     if (r.x > 10) {
       throw new Error("x too large");
     }
   }

   record R {
     var x: int;

     proc init(xVal: int) throws {
       x = xVal;
       init this;
       validate(this);
     }
   }

.. BLOCK-test-chapelpost

   try {
     var f1 = new R(4);
     writeln(f1);
     var f2 = new R(11);
   } catch e: Error {
     writeln("Caught error: ", e.message());
   }

.. BLOCK-test-chapeloutput

   (x = 4)
   Caught error: x too large


As in typical procedures, if an initializer is not declared with the
``throws`` keyword, yet makes a call that throws an error, the program
will halt if errors are encountered (see
:ref:`Chapter-Error_Handling`).

Future Work
-----------

We intend to fully support throwing initializers in the future.  This will
include:

- being able to ``throw`` from anywhere in the body of an initializer
- being able to write ``try`` / ``try!`` with ``catch`` blocks anywhere in the
  body of an initializer
- being able to call functions that ``throw`` prior to ``init this``
  (see :ref:`Limitations_on_Instance_Usage_in_Initializers` for a description)
  - including ``super.init`` calls when the parent initializer throws, e.g.,

    .. code-block:: chapel

       class A {
         var x: int;

         proc init(xVal: int) throws {
           x = xVal;
           init this;
           someThrowingFunc(this);
         }
       }

       class B : A {
         var y: bool;

         proc init(xVal: int, yVal: bool) throws {
           super.init(xVal); // This call is not valid today
           y = yVal;
           init this;
         }
       }
