.. _readme-initializers:

============
Initializers
============

Chapel is transitioning from the use of constructors for class and
record types to a new strategy called initializers.  Initializers
retain the convenience of constructors for simple types and provide
additional power and flexibility for more complex types; particularly
for generic types.

A discussion of the original design and rationale is provided in `CHIP 10`_.

Chapel's 1.16.0 release, in October 2017, provided a preview
implementation of this feature.

Release 1.17.0 advances this effort.  Bugs have been fixed, features
have been added, and many components of the internal implementation of
Chapel have transitioned from explicit constructors to explicit
initializers.  A discussion of the experiences and concerns from this
effort is provided in `CHIP 23`_.  These experiences resulted in a
useful simplification for record types and an important change in the
specification for classes with inheritance.

This release continues to provide full support for constructors and it
provides significant support for initializers.  The Chapel team
remains determined to deprecate constructors as soon as initializers
are sufficiently mature.


.. _CHIP 10:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst

.. _CHIP 23:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/23.rst



Introduction
------------

Consider a simple record declaration

.. code-block:: chapel

   record LabeledPoint {
     var x:   real;
     var y:   real;
     var txt: string;
   }

   var p1 = new LabeledPoint(x = 1.0, y = 2.0);
   var p2 = new LabeledPoint(txt = 'Seattle');


For release 1.17.0 the compiler continues to generate an all-fields
constructor for this program.  As this is a record, rather than a
class, the compiler also continues to generate a default copy
constructor and a default assignment operator. These could be written
as

.. code-block:: chapel

   proc LabeledPoint(x:   real   = 0.0,
                     y:   real   = 0.0,
                     txt: string = '') {
     this.x   = x;
     this.y   = y;
     this.txt = txt;
   }

   proc LabeledPoint(other: LabeledPoint) {
     this.x   = other.x;
     this.y   = other.y;
     this.txt = other.txt;
   }

   proc = (ref dst: LabeledPoint, src: LabeledPoint) {
     dst.x   = src.x;
     dst.y   = src.y;
     dst.txt = src.txt;
   }


For simple type declarations like this, the migration from
constructors to initializers will be largely unnoticed.  At some point
in the future the compiler will generate a default all-fields
initializer and a default copy initializer.  The definition of
the default initializers will be largely the same as the default
constructors but with the inclusion of an additional builtin
method.

.. code-block:: chapel

   proc init(x:   real   = 0.0,
             y:   real   = 0.0,
             txt: string = '') {
     this.x   = x;
     this.y   = y;
     this.txt = txt;
   }

   proc init(other: LabeledPoint) {
     this.x   = other.x;
     this.y   = other.y;
     this.txt = other.txt;
   }

   proc postinit() {

   }

Separately the compiler will continue to generate the same default
assignment operator.  The ``new`` operator will perform three
steps

1. allocate space
2. invoke an appropriate version of the ``init`` method based
   on the types of the subexpressions for the ``new`` operator
3. invoke the ``postinit`` method

This effort would be largely uninteresting if it were merely a change
in name.  The benefits of initializers compared to constructors become
evident for more sophisticated types and particularly for generic
types.


The ``init`` and ``postinit`` methods are discussed in more detail
below.  This tech note discusses these methods for the simpler case of
record types in some detail, and then describes the extensions
required to support classes with inheritance.







Constructors or Initializers but not both
-----------------------------------------

In release 1.17.0, a record or class type may include user-defined
constructors or user-defined initializers but not both.  If there are
no user-defined initializers then the compiler will continue to
generate the appropriate default constructors.

If there are any user-defined initializers then the default constructors
are suppressed.  For a record type, a default copy initializer will be
generated unless it is defined by the application.  However there will
not be a default all-fields initializer.











Initializers for Records
------------------------

Field initialization may be customized by defining one or more
overloads of the ``init`` method. Consider the following definition
for LabeledPoint.  There are two overloads for the ``init`` method
that are distinguished by their signatures.  These methods cannot
return a value. The type designer can rely on the compiler to
insert default field initializations when appropriate.



.. code-block:: chapel

   record LabeledPoint {
     var x:   real   = 1.0;
     var y:   real   = 1.0;
     var txt: string = 'Unlabeled';

     proc init(_x: real, _y: real) {
       x = _x;
       y = _y;
                        // Compiler inserts txt = 'Unlabeled';
     }

     proc init(_txt: string) {
                        // Compiler inserts x = 1.0;
                        // Compiler inserts y = 1.0;
       txt = _txt;
     }
   }

   const p1 = new LabeledPoint(10.0, 20.0);
   const p2 = new LabeledPoint('London');


Fields must be initialized in field declaration order.  This reduces
ambiguity for omitted field initializations and ensures that
observable side-effects occur in a well defined order.






Fields may be initialized within a conditional statement. The same set
of fields must be initialized in every branch.  The compiler will
initialize any omitted fields in a natural way.

.. code-block:: chapel

   record Point {
     var x: real = 1.0;
     var y: real = 1.0;
     var z: real = 5.0;

     proc init(_z: real) {
       if _z < 5 {
         x = _z;
                        // Compiler inserts y = 1.0;
       } else {
                        // Compiler inserts x = 1.0;
         y = _z;
       }

       z = _z;
     }

     proc init(_x : real, _y : real) {
       if _x + _y < 8.0 {
         x = _x;
         y = _y;
       }                // Compiler inserts the else branch
                        //     else {
                        //     x = 1.0;
                        //     y = 1.0;
                        //   }

                        // Compiler inserts z = 5.0;
     }
   }






The complete method
+++++++++++++++++++

To promote safety, an ``init`` method cannot call a method on ``this``
until every field has been initialized.  Similarly an ``init`` method
cannot pass ``this`` as an actual to a function until every field has
been initialized.

The support for default field initialization introduces the potential
for confusion about the overall status of initialization.  This is
resolved by calling a builtin method named ``complete``.  Unlike the
builtin methods ``init`` and ``postinit``, this method cannot be
overridden.  Calling this method makes it clear to the developer and
the compiler that the record should be considered to be fully
initialized.  The compiler will insert any remaining default field
initializations.

.. code-block:: chapel

   record LabeledPoint {
     var x:   real;
     var y:   real;
     var txt: string;

     proc init(x: real) {
       this.x = x;

                        // Compiler inserts y   = 1.0;
                        // Compiler inserts txt = '';

       this.complete();

       writeln('In init ', this);
     }

     proc init(x: real, y: real) {
       this.x = x;
       this.y = y;
                        // Compiler inserts this.complete();
   }







Delegating to other init methods
++++++++++++++++++++++++++++++++

An overload of the ``init`` method may delegate to another
``init`` method. For example it might be convenient to define
three overloads as follows

.. code-block:: chapel

   record LabeledPoint {
     var x:   real;
     var y:   real;
     var txt: string;

     proc init(x: real, y: real, txt : string) {
       this.x   = x;
       this.y   = y;
       this.txt = txt;
                         // Compiler inserts a call to this.complete();
     }

     proc init(x: real, y: real) {
       init(x, y, 'Unlabeled');

       writeln('init 2 :- ', this);
     }

     proc init(txt: string) {
       init(1.0, 1.0, txt);

       this.someOtherMethod();
     }
   }

A field cannot be initialized more than once.  This requirement
is enforced by preventing an ``init`` method from initializing
any fields if it delegates to another ``init`` method.

The current instance is known to be fully initialized when a
call to a delegated initializer returns.  It is safe to call
non-builtin methods and to pass ``this`` as an actual to functions
without calling this.complete().

An ``init`` method may delegate to another ``init`` method within a
conditional statement.  However every branch must fully initialize the
record.  This can be accomplished by delegating to an ``init`` method
or by invoking the builtin method this.complete() within every branch.





Record Initialization vs Record Assignment
++++++++++++++++++++++++++++++++++++++++++

It is important to distinguish between initialization and assignment
within the body of a record initializer.  For background consider the
following simple examples for variable initialization and assignment

.. code-block:: chapel

   proc example(other : MyRecord) {
     var x = 10;                     // x is initialized

     x = 20;                         // Assignment operator is invoked



     var r = new MyRecord(...);      // r is initialized

     r = other;                      // Assignment operator is invoked

     r = new MyRecord(...);          // Initialization of an internal temporary
                                     // followed by assignment to r
   }

The difference between initialization and assignment is generally
unimportant for a variable with a primitive type or a class type.
There is usually little need to override the assignment operator
for these types and the default assignment operators are as efficient
as variable initialization.

The distinction is particularly important for variables with record
type when the record includes fields with class type.  In this case it
is important to consider the ownership of the class instance that
is referenced by the field.  The assignment operator for the record
must enforce the desired rules for sharing the class instance and the
class instance should be deleted when the last reference is removed.

Consider the initializer for Point3D in the following contrived
example.  Point3D includes a field with a record type.  This field is
initialized and then assigned.

.. code-block:: chapel

   record Point2D {
     var x : real;
     var y : real;

     proc init() {
       x = 0.0;
       y = 0.0;
     }

     proc init(_x : real, _y : real) {
       x = _x;
       y = _y;
     }
   }

   record Point3D {
     var p : Point2D;
     var z : real = 1.0;

     proc init(_p : Point2D, _z : real) {
       p = _p;                       // Initialize p
       z = _z;                       // Initialize z

       p = _p;                       // Assign p
     }
   }

   var p2 = new Point2D(10.0, 20.0);
   var p3 = new Point3D(p2, 30.0);

Within the ``init`` method for Point3D, the local field ``p`` is
initialized using the default copy initializer.  Later it is
assigned, in fact to the same value, using the default
assignment operator.


Now consider the following alternative and invalid definition for
Point3D.init()

.. code-block:: chapel

   record Point3D {
     var p : Point2D;
     var z : real = 1.0;

     proc init(_p : Point2D, _z : real) {
       z = _z;                       // Initialize z

       p = _p;                       // COMPILER ERROR
     }
   }

It is unclear if the type designer intended that both of these
statements should be field initializations but accidentally reversed
the initializations, or s/he intended the compiler to insert a default
field initialization before the initialization of ``z`` followed by an
assignment.  This ambiguity is addressed by signaling an error at
compile time.  Here is an alternative that clarifies that assignment
is intended

.. code-block:: chapel

   record Point3D {
     var p : Point2D;
     var z : real = 1.0;

     proc init(_p : Point2D, _z : real) {
                                     // Compiler insert p.init()
       z = _z;                       // Initialize z

       complete();

       p = _p;                       // Assignment
     }
   }




Post Initialization for Records
+++++++++++++++++++++++++++++++

A record type that defines an initializer also implements a
``postinit`` method.  This method is invoked when the ``init`` method
returns i.e. after the record is fully initialized.  The ``postinit``
method does not accept any formals and does not return a value.
The compiler-generated definition has no observable effect.

A user may override this method and customize the behavior.  Writing a
``postinit`` method provides a way for the record author to leverage
the default all-fields initializer while also specifying additional
computation to perform before returning the new object.




Initializers for Generic Records
++++++++++++++++++++++++++++++++

Consider the following record declaration

.. code-block:: chapel

   record Point {
     type t;
     var  x: t;
     var  y: t;

     ...
   }

The declaration describes a family of user-defined types rather than
a specific type.  The set of actual types that will be generated by
this declaration depends on the definition of any overloads for the
``init`` method and the set of ``new`` expressions in the program.

Now consider the following program

.. code-block:: chapel

   record Point {
     type t;
     var  x: t;
     var  y: t;

     proc init(x, y) {
       this.t = x.type;
       this.x = x;
       this.y = y;
     }
   }

   var p1 = new Point(1,   2);
   var p2 = new Point(1.0, 2.0);

This program will generate two user defined types; Point(int) and
Point(real).  In Chapel we say that the Point declaration defines a
generic type.

A user-defined type is generic if it includes at least one generic field.
A generic field is one of

1. a specified or unspecified type alias,
2. a parameter field, or
3. a var or const field that has no type and no initialization expression.


User-defined initializers provide notable flexibility for generic types
compared to user-defined constructors.  This flexibility is evident
in the range of ``init`` method that can be supported and hence for
the allowable ``new`` expressions.





Initializers for Classes
------------------------

Class types include two factors that require additional consideration;
inheritance, and dynamic dispatch for method calls.

In an application that includes a class type that does not inherit
from some other class type and that is not a base type for any other
class type, the discussion of initializers for records can be
applied directly.



Initializers or Constructors
++++++++++++++++++++++++++++

A base class can rely on initializers or constructors but not both.
Release Chapel 1.17.0 continues to rely on constructors as the default
i.e. if there are no user-defined initializers then the base class
relies on constructors.

If a base class relies on initializers then any derived classes must
rely on initializers i.e. must include user-defined initializers.
If a base class relies on constructors then derived classes may
not include user-defined initializers.



Parents before Children
+++++++++++++++++++++++

Chapel's approach to generic types and for data dependent types,
e.g. arrays, may require that a parent's fields be initialized before
those of a derived class.  This is supported by requiring that an
``init`` method to delegate to an ``init`` method of the parent class
before any local fields are initialized.






In the following example the class Point2 is a generic class that defines
two fields that are constrained to be of the same type. The class
Point3 defines a field that must also have this type.  Hence it is
necessary that the initializer for Point2 be analyzed before that
of Point3.

.. code-block:: chapel

   class Point2 {
     type t;

     var  x: t;
     var  y: t;

     proc init(x, y) {
       this.t = x.type;
       this.x = x;
       this.y = y;
     }
   }

   class Point3 : Point2 {
     var  z: t;

     proc init(x, y) {
       super.init(x, y);

                            // compiler inserts z.init()
     }

     proc init(x, y, z) {
       super.init(x, y);

       this.z = z;
     }
   }






This second example provides an example for data dependent types.  It
is required the arrays ``a`` and ``b`` share the same domain and hence
the initializer for class Base must execute before the initializer for
Derived.

.. code-block:: chapel

   class Base {
     var d : domain(1);
     var a : [d] int;

     proc init(_a : [] int) {
       d = _a.domain;
       a = _a;
     }
   }

   class Derived : Base {
     var b : [d] int;

     proc init(_a : [] int) {
       super.init(_a);

       // Compiler inserts initialization for b[]
     }
   }


An ``init`` method for a derived class may delegate to another
``init`` method for the same class so long as this leads to a
delegation to the parent class.


If an ``init`` method for a derived class does not delegate to
any ``init`` method, the compiler will insert a call to super.init()
as the first statement.









Dynamic Dispatch
++++++++++++++++

An instance of class type is not fully initialized until the ``init``
method for the most derived class returns. Care is required to ensure
that a call to a non-init method cannot access an uninitialized field.
This is achieved by updating the runtime type of the instance during
the execution of the ``init`` methods.


An ``init`` method may not call any non-init method until the parent
class has been initialized i.e. until the call to super.init(...) has
returned.  At this point the dynamic type of the instance will be that
of the parent class.  The ``init`` method can invoke any method that
is defined on the parent class and dynamic dispatch will occur in a
manner that is safe.  It is also permitted to pass ``this`` as an
actual to functions so long as the formal can be an instance of the
parent type.

If an ``init`` method invokes the this.complete() method, the dynamic
type of the instance will be updated to match the current type. It
becomes possible to invoke methods that are defined on the current type
and dynamic dispatch for methods defined on the parent type will use
the current type.





The postinit method
+++++++++++++++++++

The ``postinit`` method for the most derived type will be invoked
when the selected ``init`` method returns.  At this point the
instance will be fully initialized.  Any method calls within an
override of a ``postinit`` method will dispatch on the instance's
final type.


The ``postinit`` method for a derived class must delegate to the
``postinit`` method for the parent class.  It is most natural
to delegate to the parent class as the first statement but there
is no requirement to do so.  If an override of this method does not
delegate to the parent class, the compiler will insert a call to
super.postinit() as the first statement.











Remaining Work
--------------

With the 1.17.0 release, support for initializers is mostly stable
with a few bugs and some unimplemented features remaining.  It is
recommended that new applications with user-defined class or record
types use initializers when possible.  Please report any bugs
encountered using the guidance described at the `bugs`_ page.

.. _bugs:
   https://chapel-lang.org/docs/usingchapel/bugs.html




Compiler Generated Initializers
+++++++++++++++++++++++++++++++

Support for compiler generated initializers is considerably more
mature than it was for the 1.16.0 release.  With the 1.17.0 release
the developer-oriented flag ``--force-initializers`` will attempt to
generate default initializers for classes and records defined in
application modules.

Additionally the compiler has a mechanism that has been used to cause
classes and records that are defined in internal, standard, and
package modules to use default initializers rather than default
constructors.

While many cases are now working there are still failure cases that
require attention.




Interaction With Error Handling
+++++++++++++++++++++++++++++++

Release 1.17.0 has limited support for error handling constructs: an
initializer cannot be declared as ``throws``, and only ``try!``
statements without ``catch`` blocks are allowed in the body.

In the world where initializers can ``throw``, we will only allow child classes
to ``throw`` if the parent initializer ``throws`` (though there may be
complications with chains of initializers, such as an initializer that calls
another initializer on the type, which calls a parent initializer that
``throws``, etc.).


Noinit
++++++

The syntax for declaring a variable includes the ability to use the
``noinit`` keyword in place of an initial value.  If the variable has
record or class type this is intended to call an initializer that
customizes the meaning of this keyword for that type.  More details on
the direction for this support can be found in the `noinit section`_
of CHIP 10.

.. _noinit section:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst#noinit

