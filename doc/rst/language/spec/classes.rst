.. _Chapter-Classes:

Classes
=======

Classes are data structures with associated state and functions. A
variable of class type either refers to a class instance, or contains a
special ``nil`` value. Note that *object* is another name for a class
instance. Storage for a class instance is not necessarily tied to the
scope of the variable(s) referring to that class instance. It is
possible for multiple variables to refer to the same class instance.

The ``new-expression`` can be used to create an instance of a class
(:ref:`Class_New`). Depending on the memory management strategy, a
class instance is either deleted automatically or can be deleted using
the ``delete-statement`` (:ref:`Class_Delete`).

A class declaration (:ref:`Class_Declarations`) generates a class
type (:ref:`Class_Types`). A variable of a class type can refer
to an instance of that class or any of its derived classes.

A class is generic if it has generic fields. A class can also be generic
if it inherits from a generic class. Generic classes and fields are
discussed in :ref:`Generic_Types`.

.. _Class_Declarations:

Class Declarations
------------------

A class is defined with the following syntax: 

.. code-block:: syntax

   class-declaration-statement:
     `class' identifier class-inherit[OPT] { class-statement-list[OPT] }

   class-inherit:
     : basic-class-type

   class-statement-list:
     class-statement
     class-statement class-statement-list

   class-statement:
     variable-declaration-statement
     method-declaration-statement
     type-declaration-statement
     empty-statement

A ``class-declaration-statement`` defines a new class type symbol
specified by the identifier. It inherits from the class specified in the
``class-inherit`` clause, when provided (:ref:`Inheritance`).

The body of a class declaration consists of a sequence of statements
where each of the statements either defines a variable (called a field),
a procedure or iterator (called a method), or a type alias. In addition,
empty statements are allowed in class declarations, and they have no
effect.

If a class declaration contains a type alias or a parameter field, or it
contains a variable or constant without a specified type and without an
initialization expression, then it declares a generic class type.
Generic classes are described in :ref:`Generic_Types`.

   *Future*.

   Privacy controls for classes and records are currently not specified,
   as discussion is needed regarding its impact on inheritance, for
   instance.

.. _Class_Lifetime_and_Borrows:

Class Lifetime and Borrows
~~~~~~~~~~~~~~~~~~~~~~~~~~

The lifetime of a class instance is the time between its creation and
its deletion. It is legal to access the class fields or methods only
during its lifetime.

Each allocation of a class instance specifies a *memory management
strategy*. Four memory management strategies are available: ``owned``,
``shared``, ``borrowed``, and ``unmanaged``.

| ``owned`` and ``shared`` class instances always have their lifetime
  managed by the compiler. In other words, the compiler automatically
  calls ``delete`` on these instances to reclaim their memory. For these
  instances, ``=`` and copy initialization can result in the transfer or
  sharing of ownership. See
| https://chapel-lang.org/docs/builtins/OwnedObject.html
| and
| https://chapel-lang.org/docs/builtins/SharedObject.html
| When ``borrowed`` is used as a memory management strategy in a
  ``new-expression``, it also creates an instance that has its lifetime
  managed by the compiler (:ref:`Class_New`).

Class instances that are ``unmanaged`` have their lifetime managed
explicitly and ``delete`` must be used to reclaim their memory.

No matter the memory management strategy used, class types support
*borrowing*. A ``borrowed`` class instance refers to the same class
instance as another variable but has no impact on the lifetime of that
instance. The process of getting such a reference to an instance is
called *borrowing*.

There are several ways to borrow an instance. To borrow explicitly the
instance managed by another variable, call the ``.borrow()`` method.
Additionally, coercions are available that are equivalent to calling the
``.borrow()`` method. For example:

   *Example (borrowing.chpl)*.

   

   .. code-block:: chapel

      class C { }
      proc test() {
        var own = new owned C();   // 'own' manages the memory of the instance
        var b = own.borrow();      // 'b' refers to the same instance but has no
                                   // impact on the lifetime.

        var bc: borrowed C = own;  // 'bc' stores the result of own.borrow()
                                   // due to coercion from owned C to
                                   // borrowed C

                                   // Note that these coercions can also apply
                                   // in the context of procedure calls.

                                   // the instance referred to by 'own' is
                                   // deleted here, at the end of the containing
                                   // block.
      }

   

   .. BLOCK-test-chapelpost

      test();

The ``.borrow()`` method is available on all class types (including
``unmanaged`` and ``borrowed``) in order to support generic programming.
For nilable class types, it returns the borrowed nilable class type.

Errors due to accessing an instance after the end of its lifetime are
particularly difficult to debug. For this reason, the compiler includes
a component called the lifetime checker. It identifies some cases where
a borrowing variable can be accessed beyond the lifetime of an instance
it refers to.

   *Future*.

   The details of lifetime checking are not yet finalized or specified.
   Additional syntax to specify the lifetimes of function returns will
   probably be needed.

.. _Class_Types:

Class Types
~~~~~~~~~~~

A class type is formed by the combination of a basic class type and a
memory management strategy.



.. code-block:: syntax

   class-type:
     basic-class-type
     `owned' basic-class-type
     `shared' basic-class-type
     `borrowed' basic-class-type
     `unmanaged' basic-class-type

A basic class type is given simply by the class name for non-generic
classes. Generic classes must be instantiated to serve as a
fully-specified type, for example to declare a variable. This is done
with type constructors, which are defined in
Section \ `24.3.6 <#Type_Constructors>`__.



.. code-block:: syntax

   basic-class-type:
     identifier
     identifier ( named-expression-list )

A basic class type, including a generic class type that is not fully
specified, may appear in the inheritance lists of other class
declarations.

If no memory management strategy is indicated, the class will be
considered to have generic management.

Variables of class type cannot store ``nil`` unless the class type is
nilable (:ref:`Nilable_Classes`).

The memory management strategies have the following meaning:

-  | ``owned`` the instance will be deleted automatically when the
     ``owned`` variable goes out of scope, but only one ``owned``
     variable can refer to the instance at a time. See
   | https://chapel-lang.org/docs/builtins/OwnedObject.html

-  | ``shared`` will be deleted when all of the ``shared`` variables
     referring to the instance go out of scope. See
   | https://chapel-lang.org/docs/builtins/SharedObject.html.

-  ``borrowed`` refers to a class instance that has a lifetime managed
   by another variable.

-  ``unmanaged`` the instance must have ``delete`` called on it
   explicitly to reclaim its memory.

It is an error to apply more than one memory management strategy to a
class type. However, in some cases, generic code needs to compute a
variant of the class type using a different memory management strategy.
Casts from the class type to a different memory management strategy are
available for this purpose
(see :ref:`Explicit_Class_Conversions`).

   *Example (duplicate-management.chpl)*.

   

   .. code-block:: chapel

      class C { }
      var x: borrowed unmanaged C;

   

   .. BLOCK-test-chapeloutput

      duplicate-management.chpl:2: error: Type expression uses multiple class kinds: borrowed unmanaged

..

   *Example (changing-management.chpl)*.

   

   .. code-block:: chapel

      class C { }
      type borrowedC = borrowed C;
      type ownedC = (borrowedC:owned);

   

   .. BLOCK-test-chapelpost

      writeln(borrowedC:string);
      writeln(ownedC:string);

   

   .. BLOCK-test-chapeloutput

      borrowed C
      owned C

.. _Nilable_Classes:

Nilable Class Types
~~~~~~~~~~~~~~~~~~~

Variables of a class type cannot store ``nil`` and do not have a default
value unless the class type is nilable. To create a nilable class type,
use the postfix ``?`` operator. For example, if ``C`` is a class, then
``C?`` indicates the nilable class type with generic memory management strategy.
The ``?`` operator can be combined with memory management specifiers as
well. For example, ``borrowed C?`` indicates a nilable class using the
``borrowed`` memory management strategy. Note that the ``?`` operator
applies only to types.

A nilable type can also be created with a cast to ``class?``. For example,
if ``T`` is a class type, then ``T: class?`` indicates its nilable counterpart,
or ``T`` itself if it is already nilable. ``T: borrowed class?`` produces
the nilable ``borrowed`` variant of ``T``.

To create a non-nilable class type from a nilable class type, apply a
cast to ``class`` or to a more specific type. For example, if ``T`` is
a class type, then ``T: class`` indicates its non-nilable counterpart,
or ``T`` itself if it is already non-nilable. ``T: borrowed class``
produces the non-nilable ``borrowed`` variant of ``T``.

The postfix ``!`` operator converts a class value to a non-nilable type.
If the value is not ``nil``, it returns a copy of that value if it is
``borrowed`` or ``unmanaged``, or a borrow from it if it is ``owned``
or ``shared``. If the value is in fact ``nil``, it halts.

An alternative to ``!`` is to use a cast to a non-nilable type. Such a
cast will throw ``NilClassError`` if the value was in fact ``nil``.
See :ref:`Explicit_Class_Conversions`.

Non-nilable class types are implicitly convertible to nilable class
types. See :ref:`Implicit_Class_Conversions`.

Class methods generally expect a receiver of type ``borrowed C``
(see :ref:`Class_Methods`). Since such a class method call might
involve dynamic dispatch, it is a program error to call a class method
on a class receiver storing ``nil``. The compiler will not
resolve calls to class methods if the receiver has nilable type. If the
programmer knows that the receiver cannot store ``nil`` at that moment,
they can use ``!`` to assert that the receiver is not ``nil`` and to
convert it to the non-nilable borrowed type. For example:

   *Example (nilable-classes-bang.chpl)*.

   

   .. code-block:: chapel

      class C {
        proc method() { }
      }
      var c: owned C? = new C();

      // Invoke c.method() only when c is non-nil.
      if c != nil {
        c!.method(); // c! converts from 'owned C?' to 'borrowed C'
      }

The ``borrow()`` method is an exception. Suppose it is invoked on an
expression of a class type ``C``. It will return ``borrowed C`` for any
non-nilable ``C`` type (e.g. ``owned C``). It will return
``borrowed C?`` for any nilable ``C`` type (e.g. ``C?``).

.. _Class_Values:

Class Values
~~~~~~~~~~~~

A class value is either a reference to an instance of a class or ``nil``
(:ref:`Class_nil_value`). Class instances can be created using a
``new`` expression (:ref:`Class_New`).

For a given class type, a legal value of that type is a reference to an
instance of either that class or a class inheriting, directly or
indirectly, from that class. ``nil`` is a legal value of any non-nilable
class type.

The default value of a concrete nilable class type is ``nil``. Generic
class types and non-nilable class types do not have a default value.
For this reason, rectangular arrays of non-nilable classes cannot be
resized, since the new array values don't have a logical default
value.  For similar reasons, associative and sparse arrays of
non-nilable classes are not currently supported.

   *Example (declaration.chpl)*.

   

   .. code-block:: chapel

      class C { }
      var c : owned C?;    // c has class type owned C?, meaning
                           // the instance can be nil and is deleted automatically
                           // when it is not.
      c = new C();         // Now c refers to an initialized instance of type C.
      var c2 = c.borrow(); // The type of c2 is borrowed C?.
                           // c2 refers to the same object as c.
      class D : C {}    // Class D is derived from C.
      c = new D();      // Now c refers to an object of type D.
                        // Since c is owned, the previous is deleted.
      // the C and D instances allocated above will be reclaimed
      // at the end of this block.

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   When the variable ``c`` is declared, it initially has the value of
   ``nil``. The next statement assigned to it an instance of the class
   ``C``. The declaration of variable ``c2`` shows that these steps can
   be combined. The type of ``c2`` is also ``borrowed C?``, determined
   implicitly from the the initialization expression. Finally, an object
   of type ``owned D`` is created and assigned to ``c``.

.. _Class_nil_value:

The *nil* Value
~~~~~~~~~~~~~~~

Chapel provides ``nil`` to indicate the absence of a reference to any
object. Invoking a class method or accessing a field of the ``nil``
value results in a run-time or compile-time error.

``nil`` can be assigned to a variable of any nilable class type. There
is a restriction for using ``nil`` as the default value or the actual
argument of a function formal, or as the initializer for a variable or a
field. Such a use is disallowed when the declared type of the
formal/variable/field is non-nilable or generic, including generic
memory management.



.. code-block:: syntax

   nil-expression:
     `nil'

.. _Class_Fields:

Class Fields
~~~~~~~~~~~~

A variable declaration within a class declaration defines a *field*
within that class. Each class instance consists of one variable per each
``var`` or ``const`` field in the class.

   *Example (defineActor.chpl)*.

   The code 

   .. BLOCK-test-chapelpre

      config param cleanUp = false;

   

   .. code-block:: chapel

      class Actor {
        var name: string;
        var age: uint;
      }

   defines a new class type called ``Actor`` that has two fields: the
   string field ``name`` and the unsigned integer field ``age``.

Field access is described in :ref:`Class_Field_Accesses`.

   *Future*.

   ``ref`` fields, which are fields corresponding to variable
   declarations with ``ref`` or ``const ref`` keywords, are an area of
   future work.

.. _Class_Methods:

Class Methods
~~~~~~~~~~~~~

Methods on classes are referred to as to as *class methods*. See the
methods section :ref:`Chapter-Methods` for more information about
methods.

Within a class method, the type of ``this`` is generally the non-nilable
``borrowed`` variant of the class type. It is different for type methods
(see below) and it might be a different type if the class method is
declared as a secondary method with a type expression
(see `[Secondary_Methods_with_Type_Expressions] <#Secondary_Methods_with_Type_Expressions>`__).

For example:

   *Example (class-method-this-type.chpl)*.

   

   ::

      class C {
        proc primaryMethod() {
          assert(this.type == borrowed C);
        }
      }
      proc C.secondaryMethod() {
        assert(this.type == borrowed C);
      }
      proc (owned C?).secondaryMethodWithTypeExpression() {
        assert(this.type == owned C?);
      }

      var x:owned C? = new owned C();
      x!.primaryMethod();   // within the method, this: borrowed C
      x!.secondaryMethod(); // within the method, this: borrowed C
      x.secondaryMethodWithTypeExpression(); // within the method, this: owned C?

   .. BLOCK-test-chapelpost

      class C {
        proc primaryMethod() {
          assert(this.type == borrowed C);
        }
      }
      proc C.secondaryMethod() {
        assert(this.type == borrowed C);
      }
      proc (owned C?).secondaryMethodWithTypeExpression() {
        assert(this.type == owned C?);
      }

      var x:owned C? = new owned C();
      x!.primaryMethod();   // within the method, this: borrowed C
      x!.secondaryMethod(); // within the method, this: borrowed C
      x.secondaryMethodWithTypeExpression(); // within the method, this: owned C?


For type methods on a class, ``this`` will accept any management or
nilability variant of the class type and it will refer to that type in
the body of the method. For example:

   *Example (class-type-method-this.chpl)*.

   

   .. code-block:: chapel

      class C {
        proc type typeMethod() {
          writeln(this:string); // print out the type of 'this'
        }
      }
      (C).typeMethod(); // prints 'C'
      (owned C).typeMethod(); // prints 'owned C'
      (borrowed C?).typeMethod(); // prints 'borrowed C?'

   

   .. BLOCK-test-chapeloutput

      C
      owned C
      borrowed C?

When a type method is defined only in a parent class, the type will be
the corresponding variant of the parent class. For example:

   *Example (class-type-method-inherit.chpl)*.

   

   .. code-block:: chapel

      class Parent { }
      class Child : Parent { }
      proc type Parent.typeMethod() {
        writeln(this:string); // print out the type 'this'
      }

      Child.typeMethod(); // prints 'Parent'
      (borrowed Child?).typeMethod(); // prints 'borrowed Parent?'

   

   .. BLOCK-test-chapeloutput

      Parent
      borrowed Parent?

.. _Nested_Classes:

Nested Classes
~~~~~~~~~~~~~~

A class defined within another class or record is a nested class. A
nested class can be referenced only within its immediately enclosing
class or record.

.. _Inheritance:

Inheritance
-----------

A class inherits, or *derives*, from the class specified in the class
declaration’s ``class-inherit`` clause when such clause is present.
Otherwise the class inherits from the predefined ``object`` class
(:ref:`The_object_Class`). In either case, there is exactly one
*parent* class. There can be many classes that inherit from a particular
parent class.

It is possible for a class to inherit from a generic class. Suppose for
example that a class ``C`` inherits from class ``ParentC``. In this
situation, ``C`` will have type constructor arguments based upon generic
fields in the ``ParentC`` as described in
 `24.3.6 <#Type_Constructors>`__. Furthermore, a fully specified ``C``
will be a subclass of a corresponding fully specified ``ParentC``.

.. _The_object_Class:

The *object* Class
~~~~~~~~~~~~~~~~~~

All classes are derived from the ``object`` class, either directly or
indirectly. If no class name appears in ``class-inherit`` clause, the
class derives implicitly from ``object``. Otherwise, a class derives
from ``object`` indirectly through the class it inherits. A variable of
type ``object`` can hold a reference to an object of any class type.

.. _Accessing_Base_Class_Fields:

Accessing Base Class Fields
~~~~~~~~~~~~~~~~~~~~~~~~~~~

A derived class contains data associated with the fields in its base
classes. The fields can be accessed in the same way that they are
accessed in their base class unless a getter method is overridden in the
derived class, as discussed
in :ref:`Overriding_Base_Class_Methods`.

.. _Shadowing_Base_Class_Fields:

Shadowing Base Class Fields
~~~~~~~~~~~~~~~~~~~~~~~~~~~

A field in the derived class can be declared with the same name as a
field in the base class. Such a field shadows the field in the base
class in that it is always referenced when it is accessed in the context
of the derived class.

   *Open issue*.

   There is an expectation that there will be a way to reference the
   field in the base class but this is not defined at this time.

.. _Overriding_Base_Class_Methods:

Overriding Base Class Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If a method in a derived class is declared with a signature identical to
that of a method in a base class, then it is said to override the base
class’s method. Such methods may be considered for dynamic dispatch if
certain criteria are met. In particular, dynamic dispatch will be used
when the method receiver has a static type of the base class but refers
to an instance of a derived class type. Additionally, a method eligible
for dynamic dispatch must not be a class method (see :ref:`Class_Methods`),
must not return ``type``, and must not return ``param``.

   *Rationale*.

   Class methods, methods that return ``type``, and methods that return
   ``param`` are not considered as candidates for dynamic dispatch because
   they are resolved at compile-time based on the static type of the
   method receiver.

In order to have identical signatures, two methods must have the same
names, and their formal arguments must have the same names, intents, types,
and order.

The return type of the overriding method must either be the same as the
return type of the base class’s method or be a subclass of the base class
method’s return type.

Methods that override a base class method must be marked with the
``override`` keyword in the ``procedure-kind``. Additionally, methods
marked with ``override`` but for which there is no parent class method
with an identical signature will result in a compiler error.

   *Rationale*.

   This feature is designed to help avoid cases where class authors
   accidentally override a method without knowing it; or fail to
   override a method that they intended to due to not meeting the
   identical signature condition.

Methods without parentheses are not candidates for dynamic dispatch.

   *Rationale*.

   Methods without parentheses are primarily used for field accessors. A
   default is created if none is specified. The field accessor should
   not dispatch dynamically since that would make it impossible to
   access a base field within a base method should that field be
   shadowed by a subclass.

.. _Class_New:

Class New
---------

To create an instance of a class, use a ``new`` expression. For example:

   *Example (class-new.chpl)*.

   

   .. code-block:: chapel

      class C {
        var x: int;
      }
      var instance = new C(1);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

The new expression can be defined by the following syntax:



.. code-block:: syntax

   new-expression:
     `new' type-expression ( argument-list )

An initializer for a given class is called by placing the ``new``
operator in front of a type expression. Any initializer arguments follow
the class name in a parenthesized list.

Syntactically, the ``type-expression`` includes ``owned``, ``shared``,
``borrowed``, and ``unmanaged``. However these have important
consequences for class new expressions. In particular, suppose ``C`` is
a ``type-expression`` that results in a class type. Then:

-  ``new C()`` is the same as ``new owned C()``

-  ``new owned C()`` allocates and initializes an instance that will be
   deleted at the end of the current block unless it is transferred to
   another ``owned`` variable. It results in something of type
   ``owned C``.

-  ``new shared C()`` allocates and initializes the instance that will
   be deleted when the last ``shared`` variable referring to it goes out
   of scope. Results in something of type ``shared C``.

-  ``new borrowed C()`` allocates and initializes an instance that will
   be automatically deleted at the end of the current block. This
   process is managed by an ``owned`` temporary. Unlike
   ``new owned C()``, this results in a value of type ``borrowed C`` and
   ownership of the instance cannot be transferred out of the block. In
   other words, ``new borrowed C()`` is equivalent to 

   .. code-block:: chapel

            (new owned C()).borrow()

-  ``new unmanaged C()`` allocates and initializes an instance that must
   have ``delete`` called on it explicitly to avoid a memory leak. It
   results in something of type ``unmanaged C``.

See also :ref:`Class_Lifetime_and_Borrows` and
:ref:`Class_Types`.

.. _Class_Initializers:

Class Initializers
------------------

A ``new`` expression allocates memory for the desired class and invokes
an *initializer* method on the uninitialized memory, passing any
arguments following the class name. An initializer is implemented by a
method named ``init`` and is responsible for initializing the fields of
the class.

Any initializers declared in a program are *user-defined* initializers.
If the program declares no initializers for a class, the compiler must
generate an initializer for that class based on the types and
initialization expressions of fields defined by that class.

.. _User_Defined_Initializers:

User-Defined Initializers
~~~~~~~~~~~~~~~~~~~~~~~~~

A user-defined initializer is an initializer method explicitly declared
in the program. An initializer declaration has the same syntax as a
method declaration, with the restrictions that the name of the method
must be ``init`` and there must not be a return type specifier. When an
initializer is called, the usual function resolution mechanism
(:ref:`Function_Resolution`) is applied with the exception that
an initializer may not be virtually dispatched.

A user-defined initializer is responsible for initializing all fields.
An initializer may omit initialization of fields, but all fields that
are initialized must be initialized in declaration order.

Initializers for generic classes (:ref:`Generic_Types`) handle
generic fields without default values differently and may need to
satisfy additional requirements. See
Section \ `24.3.9 <#Generic_User_Initializers>`__ for details.

   *Example (simpleInitializers.chpl)*.

   The following example shows a class with two initializers:
   

   .. code-block:: chapel

      class MessagePoint {
        var x, y: real;
        var message: string;

        proc init(x: real, y: real) {
          this.x = x;
          this.y = y;
          this.message = "a point";
        }

        proc init(message: string) {
          this.x = 0;
          this.y = 0;
          this.message = message;
        }
      }  // class MessagePoint

      // create two objects
      var mp1 = new MessagePoint(1.0, 2.0);
      var mp2 = new MessagePoint("point mp2");

   

   .. BLOCK-test-chapelpost

      writeln(mp1);
      writeln(mp2);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 1.0, y = 2.0, message = a point}
      {x = 0.0, y = 0.0, message = point mp2}

   The first initializer lets the user specify the initial coordinates
   and the second initializer lets the user specify the initial message
   when creating a MessagePoint.

.. _Field_Initialization_Versus_Assignment:

Field Initialization Versus Assignment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Within the body of an initializer, the first use of a field as the
left-hand side of an assignment statement will be considered
initialization. Subsequent uses of the assignment operator on the field
will invoke regular assignment as defined by the language.

   *Example (fieldInitAssignment.chpl)*.

   The following example documents the difference between field
   initialization and field assignment. 

   .. code-block:: chapel

      class PointDoubleX {
        var x, y : real;

        proc init(x: real, y: real) {
          this.x = x;              // initialization
          writeln("x = ", this.x); // use of initialized field
          this.x = this.x * 2;     // assignment, use of initialized field

          this.y = y;              // initialization
        }
      }

      var p = new PointDoubleX(1.0, 2.0);

   

   .. BLOCK-test-chapelpost

      writeln(p);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      x = 1.0
      {x = 2.0, y = 2.0}

   The first statement in the initializer initializes field ``x`` to the
   value of the formal ``x``. The second statement simply reads the
   value of the initialized field. The third statement reads the value
   of the field, doubles it, and *assigns* the result to the field
   ``x``.

If a field is used before it is initialized, an compile-time error will
be issued.

   *Example (usedBeforeInitialized.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Point {
        var x, y : real;

        proc init(x: real, y: real) {
          writeln(this.x); // Error: use of uninitialized field!
          this.x = x;
          this.y = y;
          writeln(this.y);
        }
      }
      var p = new Point(1.0, 2.0);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   The first statement in the initializer reads the value of
   uninitialized field ``x``, so the compiler will issue an error:

   

   .. code-block:: printoutput

      usedBeforeInitialized.chpl:4: In initializer:
      usedBeforeInitialized.chpl:5: error: field "x" used before it is initialized

.. _Omitting_Field_Initializations:

Omitting Field Initializations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to support productive and elegant initializers, the language
allows field initializations to be omitted if the field has a type or if
the field has an initialization expression. The compiler will insert
initialization statements for such fields based on their types and
default values.

   *Example (fieldInitOmitted.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class LabeledPoint {
        var x : real;
        var y : real;
        var msg : string = 'Unlabeled';

        proc init(x: real, y: real) {
          this.x = x;
          this.y = y;
          // compiler inserts "this.msg = 'Unlabeled'";
        }

        proc init(msg : string) {
          // compiler inserts "this.x = 0.0;"
          // compiler inserts "this.y = 0.0;"
          this.msg = msg;
        }
      }

      var A = new LabeledPoint(2.0, 3.0);
      var B = new LabeledPoint("Origin");

   

   .. BLOCK-test-chapelpost

      writeln(A);
      writeln(B);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 2.0, y = 3.0, msg = Unlabeled}
      {x = 0.0, y = 0.0, msg = Origin}

   The first initializer initializes the values of fields ``x`` and
   ``y``, and the compiler inserts initialization for the ``msg`` field
   by using its default value. The second initializer initializes the
   ``msg`` field, and the compiler inserts initialization for fields
   ``x`` and ``y`` based on the type of those fields
   (:ref:`Default_Values_For_Types`).

In order to reduce ambiguity and to ensure a well-defined order for
side-effects, the language requires that all fields be initialized in
field declaration order. This applies regardless of whether field
initializations are omitted from the initializer body. If fields are
initialized out of order, a compile-time error will be issued.

   *Example (fieldsOutOfOrder.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Point3D {
        var x = 1.0;
        var y = 1.0;
        var z = 1.0;

        proc init(x: real) {
          this.x = x;
          // compiler inserts "this.y = 1.0;"
          this.z = y * 2.0;
        }

        proc init(x: real, y: real, z: real) {
          this.x = x;
          this.z = z;
          this.y = y; // Error!
        }
      }

      var A = new Point3D(1.0);
      var B = new Point3D(1.0, 2.0, 3.0);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      fieldsOutOfOrder.chpl:12: In initializer:
      fieldsOutOfOrder.chpl:15: error: Field "y" initialized out of order
      fieldsOutOfOrder.chpl:15: note: initialization of fields before .init() call must be in field declaration order

   The first initializer leverages the well-defined order of omitted
   field initialization to use the default value of field ``y`` in order
   to explicitly initialize field ``z``.

   The second initializer initializes field ``z`` before field ``y``,
   causing a compile-time error to be issued.

..

   *Rationale*.

   Without this rule the compiler could insert default initialization
   for field ``y`` before ``z`` is explicitly initialized. The following
   statement would then be *assignment* to field ``y``, despite
   appearing to be initialization. This subtle difference may be
   confusing and surprising, and is avoided by requiring fields to be
   initialized in field declaration order.

.. _Limitations_on_Instance_Usage_in_Initializers:

Limitations on Instance Usage in Initializers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As the initializer makes progress, the class instance is incrementally
initialized. In order to prevent usage of uninitialized memory, there
are restrictions on usage of the class instance before it is fully
initialized:

-  Methods may not be invoked on partially-initialized instances

-  ``this`` may not be passed to functions while partially-initialized

These rules allow all methods and functions to assume that class
instances have been initialized, provided their value is not ``nil``.

   *Rationale*.

   The compiler could conceivably attempt to analyze methods and
   functions to determine which fields are used, and selectively allow
   method calls on partially-initialized class instances. Instead, it is
   simpler for the language to forbid method calls on
   partially-initialized instances.

Methods may be called and ``this`` may be passed to functions only after
the built-in ``complete`` method is invoked. This method may not be
overridden. If any fields have not been initialized by the time the
``complete`` method is invoked, they will be considered omitted and the
compiler will insert initialization statements as described earlier. If
the user does not invoke the ``complete`` method explicitly, the
compiler will insert a call to ``complete`` at the end of the
initializer.

   *Rationale*.

   Due to support for omitted field initialization, there is potential
   for confusion regarding the overall status of initialization. This
   confusion is addressed in the design by requiring ``complete`` to
   explicitly mark the transition between partially and fully
   initialized instances.

..

   *Implementors’ note*.

   Even if the user explicitly initializes every field, the ``complete``
   method is still required to invoke other methods.

   *Example (thisDotComplete.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class LabeledPoint {
        var x, y : real;
        var max = 10.0;
        var msg : string = 'Unlabeled';

        proc init(x: real, y: real) {
          this.x = x;
          this.y = y;
          // compiler inserts initialization for 'max' and 'msg'

          this.complete(); // 'this' is now considered to be fully initialized

          this.verify();
          writeln(this);
        }

        proc init(msg : string) {
          // compiler inserts initialization for fields 'x', 'y', and 'max'
          this.msg = msg;

          // Illegal: this.verify();
          // Implicit 'this.complete();'
        }

        proc verify() {
          if x > max || y > max then
            halt("LabeledPoint out of bounds!");
        }
      }

      var A = new LabeledPoint(1.0, 2.0);
      var B = new LabeledPoint("Origin");

   

   .. BLOCK-test-chapelpost

      writeln(B);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 1.0, y = 2.0, max = 10.0, msg = Unlabeled}
      {x = 0.0, y = 0.0, max = 10.0, msg = Origin}

   The first initializer leverages the ``complete`` method to initialize
   the remaining fields and to allow for the usage of the ``verify``
   method. Calling the ``verify`` method or passing ``this`` to
   ``writeln`` before the ``complete`` method is called would result in
   a compile-time error.

   The second initializer exists to emphasize the rule that even though
   all fields are initialized after the initialization of the ``msg``
   field, the compiler does not consider the type initialized until the
   ``complete`` method is called. If the second initializer tried to
   invoke the ``verify`` method, a compile-time error would be issued.

.. _Invoking_Other_Initializers:

Invoking Other Initializers
^^^^^^^^^^^^^^^^^^^^^^^^^^^

In order to allow for code-reuse, an initializer may invoke another
initializer implemented for the same type. Because the invoked
initializer must operate on completely uninitialized memory, a
compile-time error will be issued for field initialization before a call
to ``init``. Because each initializer either explicitly or implicitly
invokes the ``complete`` method, all fields and methods may be used
after such a call to ``init``.

   *Example (thisDotInit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Point3D {
        var x, y, z : real;

        proc init(x: real, y: real, z: real) {
          this.x = x;
          this.y = y;
          this.z = z;
          // implicit 'this.complete();'
        }

        proc init(u: real) {
          this.init(u, u, u);
          writeln(this);
        }
      }

      var A = new Point3D(1.0);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 1.0, y = 1.0, z = 1.0}

   The second initializer leverages the first initializer to initialize
   all fields with the same value. After the ``init`` call the type is
   fully initialized, the ``complete`` method has been invoked, and so
   ``this`` can be passed to the ``writeln`` function.

.. _Initializing_Fields_in_Conditional_Statements:

Initializing Fields in Conditional Statements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fields may be initialized inside of conditional statements, with the
restriction that the same set of fields must be initialized in every
branch. If the user omits any field initializations, the compiler will
insert field initializations up to and including the field furthest in
field declaration order between the conditional branches. If the else
branch of a conditional statement is omitted, the compiler will generate
an empty else branch and insert field initialization statements as
needed.

   *Example (initFieldConditional.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Point {
        var x, y : real;
        var r, theta : real;

        proc init(polar : bool, val : real) {
          if polar {
            // compiler inserts initialization for fields 'x' and 'y'
            this.r = val;
          } else {
            this.x = val;
            this.y = val;
            // compiler inserts initialization for field 'r'
          }
          // compiler inserts initialization for field 'theta'
        }
      }

      var A = new Point(true, 5.0);
      var B = new Point(false, 1.0);

   

   .. BLOCK-test-chapelpost

      writeln(A);
      writeln(B);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 0.0, y = 0.0, r = 5.0, theta = 0.0}
      {x = 1.0, y = 1.0, r = 0.0, theta = 0.0}

   The compiler identifies field ``r`` as the latest field in both
   branches, and inserts omitted field initialization statements as
   needed to ensure that fields ``x``, ``y``, and ``r`` are all
   initialized by the end of the conditional.

Conditionals may also contain calls to parent initializers
(:ref:`Initializing_Inherited`) and other initializers defined
for the current type, provided that the initialization state is the same
at the end of the conditional statement.

   *Example (thisDotInitConditional.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Parent {
        var x, y : real;
      }

      class Child : Parent {
        var z : real;

        proc init(cond : bool, val : real) {
          if cond {
            super.init(val, val);
            this.z = val;
            this.complete();
          } else {
            this.init(val, val, val);
          }
        }

        proc init(x: real, y: real, z: real) {
          super.init(x, y);
          this.z = z;
        }
      }

      var c = new Child(true, 5.0);

   

   .. BLOCK-test-chapelpost

      writeln(c);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 5.0, y = 5.0, z = 5.0}

   The first initializer must invoke the ``complete`` method at the end
   of the if-branch in order to match the state at the end of the
   else-branch.

Miscellaneous Field Initialization Rules
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Fields may not be initialized within loop statements or parallel
statements.

.. _The_Compiler_Generated_Initializer:

The Compiler-Generated Initializer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A compiler-generated initializer for a class is created automatically if
there are no initializers for that class in the program. The
compiler-generated initializer has one argument for every field in the
class, each of which has a default value equal to the field’s default
value (if present) or the default value of the field’s type (if not).
The order and names of arguments matches the order and names of field
declarations within the class.

Generic fields are discussed in
Section :ref:`Generic_Compiler_Generated_Initializers`.

The compiler-generated initializer will initialize each field to the
value of the corresponding actual argument.

   *Example (defaultInitializer.chpl)*.

   Given the class 

   .. code-block:: chapel

      class C {
        var x: int;
        var y: real = 3.14;
        var z: string = "Hello, World!";
      }

   

   .. BLOCK-test-chapelpost

      var c1 = new C();
      var c2 = new C(2);
      var c3 = new C(z="");
      var c4 = new C(2, z="");
      var c5 = new C(0, 0.0, "");
      writeln((c1, c2, c3, c4, c5));

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      ({x = 0, y = 3.14, z = Hello, World!}, {x = 2, y = 3.14, z = Hello, World!}, {x = 0, y = 3.14, z = }, {x = 2, y = 3.14, z = }, {x = 0, y = 0.0, z = })

   there are no user-defined initializers for ``C``, so ``new``
   expressions will invoke ``C``\ ’s compiler-generated initializer. The
   ``x`` argument of the compiler-generated initializer has the default
   value ``0``. The ``y`` and ``z`` arguments have the default values
   ``3.14`` and ``"Hello, World!``", respectively.

   ``C`` instances can be created by calling the compiler-generated
   initializer as follows:

   -  The call ``new C()`` is equivalent to
      ``new C(0,3.14,"Hello, World!")``.

   -  The call ``new C(2)`` is equivalent to
      ``new C(2,3.14,"Hello, World!")``.

   -  The call ``new C(z="")`` is equivalent to ``new C(0,3.14,"")``.

   -  The call ``new C(2, z="")`` is equivalent to ``new C(2,3.14,"")``.

   -  The call ``new C(0,0.0,"")`` specifies the initial values for all
      fields explicitly.

.. _The_postinit_Method:

The postinit Method
~~~~~~~~~~~~~~~~~~~

The compiler-generated initializer is powerful and flexible, but cannot
satisfy all initialization patterns desired by users. One way for users
to leverage the compiler-generated initializer while adding their own
functionality is to implement a method named ``postinit``. The
``postinit`` method may also be implemented for types with user-defined
initializers.

The compiler will insert a call to the ``postinit`` method after the
initializer invoked by the ``new`` expression finishes, if the method
exists. The ``postinit`` method accepts zero arguments and may not
return anything. Otherwise, this method behaves like any other method.

   *Example (postinit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Point3D {
        var x, y : real;
        var max = 10.0;

        proc postinit() {
          verify();
        }

        proc verify() {
          writeln("(", x, ", ", y, ")");
          if x > max || y > max then
            writeln("  Point out of bounds!");
        }
      }

      var A = new Point3D();
      var B = new Point3D(1.0, 2.0);
      var C = new Point3D(y=5.0);
      var D = new Point3D(50.0, 50.0);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   Each of the ``new`` expressions invokes the compiler-generated
   initializer, then invokes the ``verify`` method via the ``postinit``
   method: 

   .. code-block:: printoutput

      (0.0, 0.0)
      (1.0, 2.0)
      (0.0, 5.0)
      (50.0, 50.0)
        Point out of bounds!

For classes that inherit, the user may invoke the parent’s ``postinit``
method or let the compiler insert a call automatically
(:ref:`The_postinit_Method_for_Inheriting_Classes`).

.. _Initializing_Inherited:

Initializing Inherited Classes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

User-defined initializers also allow for control over initialization of
parent classes. All the fields of the parent type must be initialized
before any fields of the child type, otherwise a compile-time error is
issued. This allows for parent fields to be used in the definition of
child fields. An initializer may invoke a parent’s initializer using the
``super`` keyword.

If the user does not explicitly call the parent’s initializer, the
compiler will insert a call to the parent initializer with zero
arguments at the start of the initializer.

   *Example (simpleSuperInit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class A {
        var a, b : real;

        proc init() {
          this.init(1.0);
        }

        proc init(val : real) {
          this.a = val;
          this.b = val * 2;
        }
      }

      class B : A {
        var x, y : real;

        proc init(val: real, x: real, y: real) {
          super.init(val);
          this.x = x;
          this.y = y;
        }

        proc init() {
          // implicit super.init();
          this.x = a*2;
          this.y = b*2;
        }
      }

      var b1 = new B(4.0, 1.0, 2.0);
      var b2 = new B();

   

   .. BLOCK-test-chapelpost

      writeln(b1);
      writeln(b2);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {a = 4.0, b = 8.0, x = 1.0, y = 2.0}
      {a = 1.0, b = 2.0, x = 2.0, y = 4.0}

   The first initializer explicitly calls an initializer for class
   ``A``. Once the parent’s initializer is complete, fields of class
   ``B`` may be initialized.

   The second initializer implicitly invokes the parent’s initializer
   with zero arguments, and then uses the parent’s fields to initialize
   its own fields.

As stated earlier, the compiler will insert a zero-argument call to the
parent’s initializer if the user has not explicitly written one
themselves. The exception to this rule is if the initializer body
invokes another initializer on the current type
(:ref:`Invoking_Other_Initializers`). This other initializer
will either contain an implicit or explicit call to the parent
initializer, and so the calling initializer should not attempt to
initialize the parent itself. This also means that parent fields may not
be accessed before explicit calls to ``init``.

   *Example (superInitThisInit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Parent {
        var x, y: real;
      }

      class Child : Parent {
        var z : real;

        proc init(x: real, y: real, z: real) {
          super.init(x, y);
          this.z = z;
        }

        proc init(z: real) {
          this.init(0.0, 0.0, z);
        }
      }

      var c = new Child(5.0);

   

   .. BLOCK-test-chapelpost

      writeln(c);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 0.0, y = 0.0, z = 5.0}

   The second initializer does not contain an implicit call to the
   parent’s initializer because it explicitly invokes another
   initializer.

.. _Calling_Methods_on_Parent_Classes:

Calling Methods on Parent Classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Once ``super.init()`` returns, the dynamic type of ``this`` is the
parent’s type until the ``complete`` method
(:ref:`Limitations_on_Instance_Usage_in_Initializers`) is
invoked (except when the child’s fields are initialized and used). As a
result, the parent’s methods may be called and ``this`` may be passed to
functions as though it were of the parent type.

   *Rationale*.

   After ``super.init()`` returns the instance is in some
   partially-initialized, but valid, state. Allowing ``this`` to be
   treated as the parent allows for additional functionality and
   flexibility for users.

..

   *Example (dynamicThisInit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Parent {
        var x, y : real;

        proc foo() {
          writeln("Parent.foo");
        }
      }

      class Child : Parent {
        var z : real;

        proc init(x: real, y: real, z: real) {
          super.init(x, y); // parent's compiler-generated initializer
          foo(); // Parent.foo()
          this.z = z;
          this.complete();
          foo(); // Child.foo()
        }

        override proc foo() {
          writeln("Child.foo");
        }
      }

      var c = new Child(1.0, 2.0, 3.0);

   

   .. BLOCK-test-chapelpost

      writeln(c);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   Once the parent’s initializer is finished, the parent method ``foo``
   may be called. After the ``complete`` method is invoked, a call to
   ``foo`` resolves to the child’s overridden
   (:ref:`Overriding_Base_Class_Methods`) implementation:
   

   .. code-block:: printoutput

      Parent.foo
      Child.foo
      {x = 1.0, y = 2.0, z = 3.0}

.. _The_Compiler_Generated_Initializer_for_Inheriting_Classes:

The Compiler Generated Initializer for Inheriting Classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The compiler-generated initializer for inheriting classes will have
arguments with default values and names based on the field declarations
in the parent class. Formals for the parent type will be listed before
formals for the child type.

   *Example (compilerGeneratedInheritanceInit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Parent {
        var x, y: real;
      }

      class Child : Parent {
        var z : real;
      }

      var A = new Child();
      var B = new Child(1.0, 2.0, 3.0); // x=1.0, y=2.0, z=3.0
      var C = new Child(y=10.0);

   

   .. BLOCK-test-chapelpost

      writeln(A);
      writeln(B);
      writeln(C);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      {x = 0.0, y = 0.0, z = 0.0}
      {x = 1.0, y = 2.0, z = 3.0}
      {x = 0.0, y = 10.0, z = 0.0}

   Any ``new`` expressions using the ``Child`` type can invoke an
   initializer with three formals named ``x``, ``y``, and ``z`` that all
   have default values based on their types.

.. _The_postinit_Method_for_Inheriting_Classes:

The postinit Method for Inheriting Classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``postinit`` method on inheriting classes allows users to invoke the
parent’s ``postinit`` method using the ``super`` keyword. If the user
does not explicitly invoke the parent’s ``postinit``, the compiler will
insert the call at the top of the user’s ``postinit`` method. If the
parent type has a ``postinit`` method but the inheriting class does not,
the compiler will generate a ``postinit`` method that simply invokes the
parent’s ``postinit`` method.

   *Example (inheritancePostinit.chpl)*.

   In the following code: 

   .. code-block:: chapel

      class Parent {
        var a, b : real;
        proc postinit() {
          writeln("Parent.postinit: ", a, ", ", b);
        }
      }

      class Child : Parent {
        var x, y : real;
        proc postinit() {
          // compiler inserts "super.postinit();"
          writeln("Child.postinit: ", x, ", ", y);
        }
      }

      var c = new Child(1.0, 2.0, 3.0, 4.0);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   The compiler inserts a call to the parent’s ``postinit`` method in
   the child’s ``postinit`` method, and invokes the child’s ``postinit``
   method after the compiler-generated initializer finishes:
   

   .. code-block:: printoutput

      Parent.postinit: 1.0, 2.0
      Child.postinit: 3.0, 4.0

.. _Class_Field_Accesses:

Field Accesses
--------------

The field in a class is accessed via a field access expression.



.. code-block:: syntax

   field-access-expression:
     receiver-clause[OPT] identifier

   receiver-clause:
     expression .

The receiver-clause specifies the *receiver*, which is the class
instance whose field is being accessed. The receiver clause can be
omitted when the field access is within a method. In this case the
receiver is the method’s receiver. The receiver clause can also be
omitted when the field access is within a class declaration. In this
case the receiver is the instance being implicitly defined or
referenced.

The identifier in the field access expression indicates which field is
accessed.

A field can be modified via an assignment statement where the left-hand
side of the assignment is a field access expression.

Accessing a parameter or type field returns a parameter or type,
respectively. In addition to being available for access with a class
instance receiver, parameter and type fields can be accessed from the
instantiated class type itself.

   *Example (useActor1.chpl)*.

   Given a variable ``anActor`` of type ``Actor`` as defined above, the
   code 

   .. BLOCK-test-chapelpre

      use defineActor;
      var anActor = new unmanaged Actor(name="Tommy", age=26);

   

   .. code-block:: chapel

      var s: string = anActor.name;
      anActor.age = 27;

   

   .. BLOCK-test-chapelpost

      writeln((s, anActor));
      if (cleanUp) then delete anActor;

   

   .. BLOCK-test-chapelcompopts

      -scleanUp=true

   

   .. BLOCK-test-chapeloutput

      (Tommy, {name = Tommy, age = 27})

   reads the field ``name`` and assigns the value to the variable ``s``,
   and assigns the field ``age`` in the object ``anActor`` the value
   ``27``.

.. _Getter_Methods:

Variable Getter Methods
~~~~~~~~~~~~~~~~~~~~~~~

All field accesses are performed via getters. A getter is a method
without parentheses with the same name as the field. It is defined in
the field’s class and has a ``ref`` return intent
(:ref:`Ref_Return_Intent`). If the program does not define it,
the default getter, which simply returns the field, is provided.

   *Example (getterSetter.chpl)*.

   In the code 

   .. code-block:: chapel

      class C {
        var setCount: int;
        var x: int;
        proc x ref {
          setCount += 1;
          return x;
        }
        proc x {
          return x;
        }

      }

   

   .. BLOCK-test-chapelpost

      var c = new C();
      c.x = 1;
      writeln(c.x);
      c.x = 2;
      writeln(c.x);
      c.x = 3;
      writeln(c.x);
      writeln(c.setCount);

   

   .. BLOCK-test-chapelcompopts

      --no-warnings

   

   .. BLOCK-test-chapeloutput

      1
      2
      3
      3

   an explicit variable getter method is defined for field ``x``. It
   returns the field ``x`` and increments another field that records the
   number of times x was assigned a value.

.. _Class_Method_Calls:

Class Method Calls
------------------

Class method calls are similar to other method calls which are described
in :ref:`Method_Calls`. However, class method calls are subject
to dynamic dispatch.

The receiver-clause (or its absence) specifies the method’s receiver in
the same way it does for field accesses
:ref:`Class_Field_Accesses`.

See (:ref:`Method_receiver_and_this`) for more details of about
method receivers.

Common Operations
-----------------

.. _Class_Assignment:

Class Assignment
~~~~~~~~~~~~~~~~

Classes are assigned by reference. After an assignment from one variable
of a class type to another, both variables reference the same class
instance. Assignments from an ``owned`` variable to another ``owned`` or
``shared`` variable are an exception. They transfer ownership, leaving
the source variable empty i.e. storing ``nil``.

   *Example (owned-assignment.chpl)*.

   

   .. BLOCK-test-chapelpre

      class C { }

   

   .. code-block:: chapel

      // assume that C is a class
      var a:owned C? = new owned C();
      var b:owned C?; // default initialized to store `nil`
      b = a; // transfers ownership from a to b
      writeln(a); // a is left storing `nil`

   

   .. BLOCK-test-chapeloutput

      nil

In contrast, assignment for ``shared`` variables allows both variables
to refer to the same class instance.

The following assignments between variables or expressions with
different memory management strategies are disallowed:

-  to ``owned`` from ``shared`` or ``borrowed``, as it would not ensure
   unique ownership of the instance

-  to ``shared`` from ``borrowed``, as the original owner would be
   unaware of the shared ownership

-  to ``owned``, ``shared``, or ``borrowed`` from ``unmanaged``, as both
   the source and the destination would appear responsible for deleting
   the instance

.. _Class_Delete:

Deleting Unmanaged Class Instances
----------------------------------

Memory associated with ``unmanaged`` class instances can be reclaimed
with the ``delete`` statement:



.. code-block:: syntax

   delete-statement:
     `delete' expression-list ;

where the expression-list specifies the class objects whose memory will
be reclaimed. Prior to releasing their memory, the deinitialization
routines for these objects will be executed
(:ref:`Class_Deinitializer`). The expression-list can contain
array expressions, in which case each element of that array will be
deleted in parallel using a ``forall`` loop over the array. It is legal
to delete a class variable whose value is ``nil``, though this has no
effect. If a class instance is referenced after it has been deleted, the
behavior is undefined.

   *Example (delete.chpl)*.

   The following example allocates a new object ``c`` of class type
   ``C`` and then deletes it. 

   .. BLOCK-test-chapelpre

      class C {
        var i,j,k: int;
      }

   

   .. code-block:: chapel

      var c : unmanaged C? = nil;
      delete c;              // Does nothing: c is nil.

      c = new unmanaged C(); // Creates a new object.
      delete c;              // Deletes that object.

      // The following statements reference an object after it has been deleted, so
      // the behavior of each is "undefined":
      // writeln(c.i); // May read from freed memory.
      // c.i = 3;      // May overwrite freed memory.
      // delete c;     // May confuse some allocators.

   .. BLOCK-test-chapelpost

      writeln("DONE");

   .. BLOCK-test-chapelexecopts

      --memLeaksByType

   

   .. BLOCK-test-chapeloutput

      DONE

      ====================
      Leaked Memory Report
      ==============================================================
      Number of leaked allocations
                 Total leaked memory (bytes)
                            Description of allocation
      ==============================================================
      ==============================================================

.. _Class_Deinitializer:

Class Deinitializer
~~~~~~~~~~~~~~~~~~~

A class author may create a deinitializer to specify additional actions
to be performed when a class instance is deleted. A class deinitializer
is a method named ``deinit()``. It must take no arguments (aside from the
implicit ``this`` argument). If defined, the deinitializer is called each
time a ``delete`` statement is invoked with a valid instance of that
class type. The deinitializer is not called if the argument of ``delete``
evaluates to ``nil``. Note that when an ``owned`` or ``shared`` reaches
its deinit point (see :ref:`Deinit_Points`), it may call ``delete`` on a
class instance which in turn will run the deinitializer and then reclaim
the memory.

   *Example (classDeinitializer.chpl)*.

   

   .. code-block:: chapel

      class C {
        var i: int;
        proc deinit() { writeln("Bye, bye ", i); }
      }

      var c : unmanaged C? = nil;
      delete c;              // Does nothing: c is nil.

      c = new unmanaged C(1); // Creates a new instance.
      delete c;               // Deletes that instance: Writes out "Bye, bye 1"
                              // and reclaims the memory that was held by c.
      {
        var own = new owned C(2); // Creates a new owned instance
                                  // The instance is automatically deleted at
                                  // the end of this block, so "Bye, bye 2" is
                                  // output and then the memory is reclaimed.
      }

   

   .. BLOCK-test-chapeloutput

      Bye, bye 1
      Bye, bye 2
