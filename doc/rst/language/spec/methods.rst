.. _Chapter-Methods:

Methods
=======

A *method* is a procedure or iterator that is associated with an
expression known as the *receiver*.

Methods are declared with the following syntax: 

.. code-block:: syntax

   method-declaration-statement:
     procedure-kind[OPT] proc-or-iter this-intent[OPT] type-binding[OPT] function-name argument-list[OPT]
       return-intent[OPT] return-type[OPT] where-clause[OPT] function-body

   proc-or-iter:
     `proc'
     `iter'

   this-intent:
     `param'
     `type'
     `ref'
     `const ref'
     `const'

   type-binding:
     identifier .
     `(' expr `)' .

Methods defined within the lexical scope of a class, record, or union
are referred to as *primary methods*. For such methods, the
``type-binding`` is omitted and is taken to be the innermost class,
record, or union in which the method is defined.

Methods defined outside of such scopes are known as *secondary methods*
and must have a ``type-binding`` (otherwise, they would simply be
standalone functions rather than methods). Note that secondary methods
can be defined not only for classes, records, and unions, but also for
any other type (e.g., integers, reals, strings).

[Secondary_Methods_with_Type_Expressions] Secondary methods can be
declared with a type expression instead of a type identifier. In
particular, if the ``type-binding`` is a parenthesized expression, the
compiler will evaluate that expression to find the receiver type for the
method. In that case, the method applies only to receivers of that type.
See also
:ref:`Creating_General_and_Specialized_Versions_of_a_Function`.

Method calls are described in :ref:`Method_Calls`.

The use of ``this-intent`` is described in
:ref:`Method_receiver_and_this`.

.. _Method_Calls:

Method Calls
------------

A method is invoked with a method call, which is similar to a non-method
call expression, but it can include a receiver clause. The receiver
clause syntactically identifies a single argument by putting it before
the method name. That argument is the method receiver. When calling a
method from another method, or from within a class or record
declaration, the receiver clause can be omitted.



.. code-block:: syntax

   method-call-expression:
     receiver-clause[OPT] expression ( named-expression-list )
     receiver-clause[OPT] expression [ named-expression-list ]
     receiver-clause[OPT] parenthesesless-function-identifier

The receiver-clause (or its absence) specifies the method’s receiver
:ref:`Method_receiver_and_this`.

   *Example (defineMethod.chpl)*.

   A method to output information about an instance of the ``Actor``
   class can be defined as follows: 

   .. BLOCK-test-chapelpre

      class Actor {
        var name: string;
        var age: uint;
      }
      var anActor = new Actor(name="Tommy", age=27);
      writeln(anActor);

   

   .. code-block:: chapel

      proc Actor.print() {
        writeln("Actor ", name, " is ", age, " years old");
      }

   

   .. BLOCK-test-chapelpost

      anActor.print();

   

   .. BLOCK-test-chapeloutput

      {name = Tommy, age = 27}
      Actor Tommy is 27 years old

   This method can be called on an instance of the ``Actor`` class,
   ``anActor``, with the call expression ``anActor.print()``.

The actual arguments supplied in the method call are bound to the formal
arguments in the method declaration following the rules specified for
procedures (:ref:`Chapter-Procedures`). The exception is the
receiver :ref:`Method_receiver_and_this`.

.. _Method_receiver_and_this:

The Method Receiver and the *this* Argument
-------------------------------------------

A method’s *receiver* is an implicit formal argument named ``this``
representing the expression on which the method is invoked. The
receiver’s actual argument is specified by the ``receiver-clause`` of a
method-call-expression as specified in :ref:`Method_Calls`.

   *Example (implicitThis.chpl)*.

   Let class ``C``, method ``foo``, and function ``bar`` be defined as
   

   .. code-block:: chapel

      class C {
        proc foo() {
          bar(this);
        }
      }
      proc bar(c: C) { writeln(c); }

   

   .. BLOCK-test-chapelpost

      var c1: C = new C();
      c1.foo();

   

   .. BLOCK-test-chapeloutput

      {}

   Then given an instance of ``C`` called ``c1``, the method call
   ``c1.foo()`` results in a call to ``bar`` where the argument is
   ``c1``. Within primary method ``C.foo()``, the (implicit) receiver
   formal has static type ``borrowed C`` and is referred to as ``this``.

Methods whose receivers are objects are called *instance methods*.
Methods may also be defined to have ``type`` receivers—these are known
as *type methods*.

Note that within a method for a class ``C``, the type of ``this`` is
generally ``borrowed C``. Within a type method on a class ``C``,
``this`` refers to the class type ``C`` with management and nilability
matching the type of the receiver. Please
see :ref:`Class_Methods` for more details.

The optional ``this-intent`` is used to specify type methods, to
constrain a receiver argument to be a ``param``, or to specify how the
receiver argument should be passed to the method.

When no ``this-intent`` is used, a default this intent applies. For
methods on classes and other primitive types, the default this intent is
the same as the default intent for that type. For record methods, the
intent for the receiver formal argument is ``ref`` or ``const ref``,
depending on whether the formal argument is modified inside of the
method. Programmers wishing to be explicit about whether or not record
methods modify the receiver can explicitly use the ``ref`` or
``const ref`` ``this-intent``.

A method whose ``this-intent`` is ``type`` defines a *type method*. It
can only be called on the type itself rather than on an instance of the
type. When ``this-intent`` is ``param``, it specifies that the function
can only be applied to param objects of the given type binding.

   *Example (paramTypeThisIntent.chpl)*.

   In the following code, the ``isOdd`` method is defined with a
   ``this-intent`` of ``param``, permitting it to be called on params
   only. The ``size`` method is defined with a ``this-intent`` of
   ``type``, requiring it to be called on the ``int`` type itself, not
   on integer values. 

   .. code-block:: chapel

      proc param int.isOdd() param {
        return this & 0x1 == 0x1;
      }

      proc type int.size() param {
        return 64;
      }

      param three = 3;
      var seven = 7;

      writeln(42.isOdd());          // prints false
      writeln(three.isOdd());       // prints true
      writeln((42+three).isOdd());  // prints true
      // writeln(seven.isOdd());    // illegal since 'seven' is not a param

      writeln(int.size());          // prints 64
      // writeln(42.size());        // illegal since 'size()' is a type method

   

   .. BLOCK-test-chapeloutput

      false
      true
      true
      64

Type methods can also be iterators.

   *Example (typeMethodIter.chpl)*.

   In the following code, the class ``C`` defines a type method iterator
   which can be invoked on the type itself: 

   .. code-block:: chapel

      class C {
        var x: int;
        var y: string;

        iter type myIter() {
          yield 3;
          yield 5;
          yield 7;
          yield 11;
        }
      }

      for i in C.myIter() do
        writeln(i);

   

   .. BLOCK-test-chapeloutput

      3
      5
      7
      11

When ``this-intent`` is ``ref``, the receiver argument will be passed by
reference, allowing modifications to ``this``. If ``this-intent`` is
``const ref``, the receiver argument is passed by reference but it
cannot be modified inside the method. The ``this-intent`` can also
describe an abstract intent as follows. If it is ``const``, the receiver
argument will be passed with ``const`` intent. If it is left out
entirely, the receiver will be passed with a default intent. For
records, that default intent is ``ref`` if ``this`` is modified within
the function and ``const ref`` otherwise. For other types, the default
``this`` intent matches the default argument intent described in
:ref:`The_Default_Intent`.

   *Example (refThisIntent.chpl)*.

   In the following code, the ``doubleMe`` function is defined with a
   ``this-intent`` of ``ref``, allowing variables of type ``int`` to
   double themselves. 

   .. code-block:: chapel

      proc ref int.doubleMe() { this *= 2; }

   

   .. BLOCK-test-chapelpost

      var x: int = 2;
      x.doubleMe();
      writeln(x);

   

   .. BLOCK-test-chapeloutput

      4

   Given a variable ``x = 2``, a call to ``x.doubleMe()`` will set ``x``
   to ``4``.

.. _The_this_Method:

The *this* Method
-----------------

A procedure method declared with the name ``this`` allows the receiver
to be “indexed” similarly to how an array is indexed. Indexing (as with
``A[1]``) has the semantics of calling a method named ``this``. There is
no other way to call a method called ``this``. The ``this`` method must
be declared with parentheses even if the argument list is empty.

   *Example (thisMethod.chpl)*.

   In the following code, the ``this`` method is used to create a class
   that acts like a simple array that contains three integers indexed by
   1, 2, and 3. 

   .. code-block:: chapel

      class ThreeArray {
        var x1, x2, x3: int;
        proc this(i: int) ref {
          select i {
            when 1 do return x1;
            when 2 do return x2;
            when 3 do return x3;
          }
          halt("ThreeArray index out of bounds: ", i);
        }
      }

   

   .. BLOCK-test-chapelpost

      var ta = new ThreeArray();
      ta(1) = 1;
      ta(2) = 2;
      ta(3) = 3;
      for i in 1..3 do
        writeln(ta(i));
      ta(4) = 4;

   

   .. BLOCK-test-chapeloutput

      1
      2
      3
      thisMethod.chpl:9: error: halt reached - ThreeArray index out of bounds: 4

.. _The_these_Method:

The *these* Method
------------------

An iterator method declared with the name ``these`` allows the receiver
to be “iterated over” similarly to how a domain or array supports
iteration. When a value supporting a ``these`` method is used as the the
``iteratable-expression`` of a loop, the loop proceeds in a manner
controlled by the ``these`` iterator.

   *Example (theseIterator.chpl)*.

   In the following code, the ``these`` method is used to create a class
   that acts like a simple array that can be iterated over and contains
   three integers. 

   .. code-block:: chapel

      class ThreeArray {
        var x1, x2, x3: int;
        iter these() ref {
          yield x1;
          yield x2;
          yield x3;
        }
      }

   

   .. BLOCK-test-chapelpost

      var ta = new ThreeArray();
      for (i, j) in zip(ta, 1..) do
        i = j;

      for i in ta do
        writeln(i);

   

   .. BLOCK-test-chapeloutput

      1
      2
      3

An iterator type method with the name ``these`` supports iteration over
the class type itself.

   *Example (typeMethodIterThese.chpl)*.

   In the following code, the class ``C`` defines a type method iterator
   named ``these``, supporting direct iteration over the type:
   

   .. code-block:: chapel

      class C {
        var x: int;
        var y: string;

        iter type these() {
          yield 1;
          yield 2;
          yield 4;
          yield 8;
        }
      }

      for i in C do
        writeln(i);

   

   .. BLOCK-test-chapeloutput

      1
      2
      4
      8
