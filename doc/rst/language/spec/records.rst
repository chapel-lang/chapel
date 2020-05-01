.. _Chapter-Records:

Records
=======

A record is a data structure that is similar to a class except it has
value semantics, similar to primitive types. Value semantics mean that
assignment, argument passing and function return values are by default
all done by copying. Value semantics also imply that a variable of
record type is associated with only one piece of storage and has only
one type throughout its lifetime. Storage is allocated for a variable of
record type when the variable declaration is executed, and the record
variable is also initialized at that time. When the record variable goes
out of scope, or at the end of the program if it is declared at module
scope, it is deinitialized and its storage is deallocated.

A record declaration statement creates a record
type :ref:`Record_Declarations`. A variable of record type
contains all and only the fields defined by that type
(:ref:`Record_Types`). Value semantics imply that the type of a
record variable is known at compile time (i.e. it is statically typed).

A record can be created using the ``new`` operator, which allocates
storage, initializes it via a call to a record initializer, and returns
it. A record is also created upon a variable declaration of a record
type.

A record type is generic if it contains generic fields. Generic record
types are discussed in detail in :ref:`Generic_Types`.

.. _Record_Declarations:

Record Declarations
-------------------

A record type is defined with the following syntax: 

.. code-block:: syntax

   record-declaration-statement:
     simple-record-declaration-statement
     external-record-declaration-statement

   simple-record-declaration-statement:
     `record' identifier { record-statement-list }

   record-statement-list:
     record-statement
     record-statement record-statement-list

   record-statement:
     variable-declaration-statement
     method-declaration-statement
     type-declaration-statement
     empty-statement

A ``record-declaration-statement`` defines a new type symbol specified
by the identifier. As in a class declaration, the body of a record
declaration can contain variable, method, and type declarations.

If a record declaration contains a type alias or parameter field, or it
contains a variable or constant field without a specified type and
without an initialization expression, then it declares a generic record
type. Generic record types are described
in :ref:`Generic_Types`.

If the ``extern`` keyword appears before the ``record`` keyword, then an
external record type is declared. An external record is used within
Chapel for type and field resolution, but no corresponding backend
definition is generated. It is presumed that the definition of an
external record is supplied by a library or the execution environment.
See the chapter on interoperability
(:ref:`Chapter-Interoperability`) for more information on
external records.

   *Future*.

   Privacy controls for classes and records are currently not specified,
   as discussion is needed regarding its impact on inheritance, for
   instance.

.. _Record_Types:

Record Types
~~~~~~~~~~~~

A record type specifier simply names a record type, using the following
syntax: 

.. code-block:: syntax

   record-type:
     identifier
     identifier ( named-expression-list )

A record type specifier may appear anywhere a type specifier is
permitted.

For non-generic records, the record name by itself is sufficient to
specify the type. Generic records must be instantiated to serve as a
fully-specified type, for example to declare a variable. This is done
with type constructors, which are defined in
Section \ `24.3.6 <#Type_Constructors>`__.

.. _Record_Fields:

Record Fields
~~~~~~~~~~~~~

Variable declarations within a record type declaration define fields
within that record type. The presence of at least one parameter field
causes the record type to become generic. Variable fields define the
storage associated with a record.

   *Example (defineActorRecord.chpl)*.

   The code 

   .. code-block:: chapel

      record ActorRecord {
        var name: string;
        var age: uint;
      }

   defines a new record type called ``ActorRecord`` that has two fields:
   the string field ``name`` and the unsigned integer field ``age``. The
   data contained by a record of this type is exactly the same as that
   contained by an instance of the ``Actor`` class defined in the
   preceding chapter :ref:`Class_Fields`.

.. _Record_Methods:

Record Methods
~~~~~~~~~~~~~~

A record method is a function or iterator that is bound to a record. See
the methods section :ref:`Chapter-Methods` for more information
about methods.

Note that the receiver of a record method is passed by ``ref`` or
``const ref`` intent by default, depending on whether or not ``this`` is
modified in the body of the method.

.. _Nested_Record_Types:

Nested Record Types
~~~~~~~~~~~~~~~~~~~

A record defined within another class or record is a nested record. A
nested record can be referenced only within its immediately enclosing
class or record.

.. _Record_Variable_Declarations:

Record Variable Declarations
----------------------------

A record variable declaration is a variable declaration using a record
type. When a variable of record type is declared, storage is allocated
sufficient to store all of the fields defined in that record type.

In the context of a class or record or union declaration, the fields are
allocated within the object as if they had been declared individually.
In this sense, records provide a way to group related fields within a
containing class or record type.

In the context of a function body, a record variable declaration causes
storage to be allocated sufficient to store all of the fields in that
record type. The record variable is initialized with a call to an
initializer (:ref:`Class_Initializers`) that accepts zero actual
arguments.

.. _Record_Storage:

Storage Allocation
~~~~~~~~~~~~~~~~~~

Storage for a record variable directly contains the data associated with
the fields in the record, in the same manner as variables of primitive
types directly contain the primitive values.  Unlike class variables, the
field data of one record variable is not shared with data of another
record variable.

Record storage is reclaimed automatically. See :ref:`Variable_Lifetimes`
for details on when a record becomes dead.

.. _Record_Initialization:

Record Initialization
~~~~~~~~~~~~~~~~~~~~~

A variable of a record type declared without an initialization
expression is initialized through a call to the record’s default
initializer, passing no arguments. The default initializer for a record
is defined in the same way as the default initializer for a class
(:ref:`The_Compiler_Generated_Initializer`).

To create a record as an expression, i.e. without binding it to a
variable, the ``new`` operator is required. In this case, storage is
allocated and reclaimed as for a record variable declaration
(:ref:`Record_Storage`), except that the temporary record goes
out of scope at the end of the enclosing block.

The initializers for a record are defined in the same way as those for a
class (:ref:`Class_Initializers`). Note that records do not
support inheritance and therefore the initializer rules for inheriting
classes (:ref:`Initializing_Inherited`) do not apply to record
initializers.

   *Example (recordCreation.chpl)*.

   The program 

   .. code-block:: chapel

      record TimeStamp {
        var time: string = "1/1/1011";
      }

      var timestampDefault: TimeStamp;                  // use the default for 'time'
      var timestampCustom = new TimeStamp("2/2/2022");  // ... or a different one
      writeln(timestampDefault);
      writeln(timestampCustom);

      var idCounter = 0;
      record UniqueID {
        var id: int;
        proc init() { idCounter += 1; id = idCounter; }
      }

      var firstID : UniqueID; // invokes zero-argument initializer
      writeln(firstID);
      writeln(new UniqueID());  // create and use a record value without a variable
      writeln(new UniqueID());

   produces the output 

   .. code-block:: printoutput

      (time = 1/1/1011)
      (time = 2/2/2022)
      (id = 1)
      (id = 2)
      (id = 3)

   The variable ``timestampDefault`` is initialized with
   ``TimeStamp``\ ’s default initializer. The expression
   ``new TimeStamp`` creates a record that is assigned to
   ``timestampCustom``. It effectively initializes ``timestampCustom``
   via a call to the initializer with desired arguments. The records
   created with ``new UniqueID()`` are discarded after they are used.

As with classes, the user can provide their own initializers
(:ref:`User_Defined_Initializers`). If any user-defined
initializers are supplied, the default initializer cannot be called
directly.

.. _Record_Deinitializer:

Record Deinitializer
~~~~~~~~~~~~~~~~~~~~

A record author may specify additional actions to be performed before
record storage is reclaimed by defining a record deinitializer. A record
deinitializer is a method named ``deinit()``. A record deinitializer
takes no arguments (aside from the implicit ``this`` argument). If
defined, the deinitializer is called on a record object after it goes
out of scope and before its memory is reclaimed.

   *Example (recordDeinitializer.chpl)*.

   

   .. code-block:: chapel

      class C { var x: int; } // A class with nonzero size.
      // If the class were empty, whether or not its memory was reclaimed
      // would not be observable.

      // Defines a record implementing simple memory management.
      record R {
        var c: unmanaged C;
        proc init() {
          c = new unmanaged C(0);
        }
        proc deinit() {
          delete c;
        }
      }

      proc foo()
      {
        var r: R; // Initialized using default initializer.
        writeln(r);
        // r will go out of scope here.
        // Its deinitializer will be called to free the C object it contains.
      }

      foo();

   .. BLOCK-test-chapeloutput

      (c = {x = 0})

      ====================
      Leaked Memory Report
      ==============================================================
      Number of leaked allocations
                 Total leaked memory (bytes)
                            Description of allocation
      ==============================================================
      ==============================================================

   .. BLOCK-test-chapelexecopts

      --memLeaksByType

.. _Record_Arguments:

Record Arguments
----------------

Record formal arguments with the ``in`` intent will be copy-initialized
into the function’s formal argument
(:ref:`Copy_Initialization_of_Records`).

Record formal arguments with ``inout`` or ``out`` intent will be updated
by the record assignment function (:ref:`Record_Assignment`).

   *Example (paramPassing.chpl)*.

   The program 

   .. code-block:: chapel

      record MyColor {
        var color: int;
      }
      proc printMyColor(in mc: MyColor) {
        writeln("my color is ", mc.color);
        mc.color = 6;   // does not affect the caller's record
      }
      var mc1: MyColor;        // 'color' defaults to 0
      var mc2: MyColor = mc1;  // mc1's value is copied into mc2
      mc1.color = 3;           // mc1's value is modified
      printMyColor(mc2);       // mc2 is not affected by assignment to mc1
      printMyColor(mc2);       // ... or by assignment in printMyColor()

      proc modifyMyColor(inout mc: MyColor, newcolor: int) {
        mc.color = newcolor;
      }
      modifyMyColor(mc2, 7);   // mc2 is affected because of the 'inout' intent
      printMyColor(mc2);

   produces 

   .. code-block:: printoutput

      my color is 0
      my color is 0
      my color is 7

   The assignment to ``mc1.color`` affects only the record stored in
   ``mc1``. The record in ``mc2`` is not affected by the assignment to
   ``mc1`` or by the assignment in ``printMyColor``. ``mc2`` is affected
   by the assignment in ``modifyMyColor`` because the intent ``inout``
   is used.

.. _Record_Field_Access:

Record Field Access
-------------------

A record field is accessed the same way as a class field
(:ref:`Class_Field_Accesses`). When a field access is used as an
rvalue, the value of that field is returned. When it is used as an
lvalue, the value of the record field is updated.

Accessing a parameter or type field returns a parameter or type,
respectively. Also, parameter and type fields can be accessed from an
instantiated record type in addition to from a record value.

.. _Field_Getter_Methods:

Field Getter Methods
~~~~~~~~~~~~~~~~~~~~

As in classes, field accesses are performed via getter methods
(:ref:`Getter_Methods`). By default, these methods simply return
a reference to the specified field (so they can be written as well as
read). The user may redefine these as needed.

.. _Record_Method_Access:

Record Method Calls
-------------------

Record method calls are written the same way as other method calls
(:ref:`Method_Calls`). Unlike class methods, record methods are
always resolved at compile time.

.. _common-operations-1:

Common Operations
-----------------

.. _Copy_Initialization_of_Records:

Copy Initialization of Records
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When a new record variable is created based upon an existing variable,
it is *copy initialized* or *move initialized* as described in
:ref:`Copy_and_Move_Initialization`. When a record is *copy initialized*,
its ``init=`` initializer will be used to create the new record.

Copy initialization is implemented by a method named ``init=``, known as the
*copy initializer*. A copy initializer may only accept one argument, which
represents the value from which the record will be initialized. These methods
share the same rules as a normal initializer (:ref:`Class_Initializers`), along
with some additional restrictions.

The compiler-generated copy initializer for a non-generic record accepts an
argument of the same type and simply initializes each field from the argument's
corresponding field:

.. code-block:: chapel

  record R {
    var x, y, z: int;
  }

  // identical to compiler-generated implementation
  // proc R.init=(other: R) {
  //   this.x = other.x;
  //   this.y = other.y;
  //   this.z = other.z;
  // }

In order to override the compiler-generated implementation, the user must
implement an ``init=`` method with the same signature.

.. code-block:: chapel

  proc R.init=(other: R) {
    this.x = other.x;
    this.y = other.y;
    this.z = other.z;
    writeln("copied R!");
  }

.. note::

  If a user implements their own ``init=`` method, they must also implement an
  assignment operator for the same record type. Implementing one without the
  other will cause the compiler to issue an error. *Rationale*: this
  requirement exists to mitigate hard-to-debug problems by requiring that type
  authors take responsibility for both ``init=`` and ``=`` implementations, or
  neither implementation.

A user may indicate that a type is not copyable by adding a where-clause to
the ``init=`` implementation that evaluates to ``false``:

.. code-block:: chapel

  proc R.init=(other: R) where false {
  }

The compiler-generated copy initializer for a generic type uses the expression
``this.type`` as the argument's type to ensure that the types of the original
record and its copy are the same:

.. code-block:: chapel

  record G {
    type T;
    var x : T;
  }

  // compiler-generated init= for 'G'
  // proc G.init=(other: this.type) {
  //   this.T = other.T;
  //   this.x = other.x;
  // }

Note that the generic fields must still be manually initialized, despite
the type already being known. Future work may allow these fields to be inferred.

.. _Advanced_Copy_Initialization:

Advanced Copy Initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A copy initializer can also be used to specify how a record should be
initialized from a value of an arbitrary type. This kind of copy initializer is
invoked when a variable declaration's initialization expression is not of the
same type as the record being initialized. For example:

.. code-block:: chapel

  record MyString {
    var s : string;
  }

  // normal copy initializer
  proc MyString.init=(other: MyString) {
    this.s = other.s;
    writeln("normal init=");
  }

  // initialize from a string
  proc MyString.init=(other: string) {
    this.s = other;
    writeln("string init=");
  }

  var A = new MyString("hello");
  var B = A; // "normal init="
  var C : MyString = "goodbye"; // "string init="

Generic types can rely on the ``this.type`` expression to implement these kinds
of copy initializers with the desired type constraints. The ``this.type``
expression will evaluate to the type provided by the user at the variable
declaration:

.. code-block:: chapel

  record Wrapper {
    type T;
    var x : T;
  }

  // normal copy initializer
  proc Wrapper.init=(other: this.type) { ... }

  // An incorrect attempt: ignores the user-specified type, and uses the
  // value's type (which might not be the same!)
  // i.e. 'var w : Wrapper(int) = "hi"', tries to create a 'Wrapper(string)'
  // proc Wrapper.init=(other: ?T) {
  //   this.T = T;
  //   this.x = other;
  // }

  // initialize a Wrapper from the desired wrapped type 'T'
  proc Wrapper.init=(other: this.type.T) {
    this.T = other.type;
    this.x = other;
  }

  var A : Wrapper(int) = 4;
  var B : Wrapper(string) = "hello";

.. _Record_Assignment:

Record Assignment
~~~~~~~~~~~~~~~~~

A variable of record type may be updated by assignment. The compiler
provides a default assignment operator for each record type ``R`` having
the signature:



.. code-block:: chapel

   proc =(ref lhs:R, rhs:R) : void where lhs.type == rhs.type;

In it, the value of each field of the record on the right-hand side is
assigned to the corresponding field of the record on the left-hand side.

The compiler-provided assignment operator may be overridden as described
in `11.3 <#Assignment_Statements>`__.

The following example demonstrates record assignment.

   *Example (assignment.chpl)*.

   

   .. code-block:: chapel

      record R {
        var i: int;
        var x: real;
        proc print() { writeln("i = ", this.i, ", x = ", this.x); }
      }
      var A: R;
      A.i = 3;
      A.print();	// "i = 3, x = 0.0"

      var C: R;
      A = C;
      A.print();	// "i = 0, x = 0.0"

      C.x = 3.14;
      A.print();	// "i = 0, x = 0.0"

   

   .. BLOCK-test-chapeloutput

      i = 3, x = 0.0
      i = 0, x = 0.0
      i = 0, x = 0.0

   Prior to the first call to ``R.print``, the record ``A`` is created
   and initialized to all zeroes. Then, its ``i`` field is set to ``3``.
   For the second call to ``R.print``, the record ``C`` is created
   assigned to ``A``. Since ``C`` is default-initialized to all zeroes,
   those zero values overwrite both values in ``A``.

   The next clause demonstrates that ``A`` and ``C`` are distinct
   entities, rather than two references to the same object. Assigning
   ``3.14`` to ``C.x`` does not affect the ``x`` field in ``A``.

.. _Record_Comparison_Operators:

Default Comparison Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Default functions to overload comparison operators are defined for
records if none are explicitly defined. ``==`` and ``!=`` functions have the
following signatures for a record ``R``:



.. code-block:: chapel

   proc ==(lhs:R, rhs:R) : bool where lhs.type == rhs.type;
   proc !=(lhs:R, rhs:R) : bool where lhs.type == rhs.type;

Other comparison operator overloads (namely ``<``, ``<=``, ``>``, and ``>=``)
have similar signatures but their where clauses also check whether the relevant
operator is supported by each field.

Record comparisons have a similar behavior to :ref:`tuple comparisons
<Tuple_Relational_Operators>`.  The operators ``>``, ``>=``, ``<``, and ``<=``
check the corresponding lexicographical order based on pair-wise comparisons
between the arguments' fields.  The operators ``==`` and ``!=`` check whether
the two arguments are pair-wise equal or not.  The fields are compared in the
order they are declared in the record definition.

.. _Class_and_Record_Differences:

Differences between Classes and Records
---------------------------------------

The key differences between records and classes are listed below.

.. _Declaration_Differences:

Declarations
~~~~~~~~~~~~

Syntactically, class and record type declarations are identical, except
that they begin with the ``class`` and ``record`` keywords,
respectively. In contrast to classes, records do not support
inheritance.

.. _Storage_Allocation_Differences:

Storage Allocation
~~~~~~~~~~~~~~~~~~

For a variable of record type, storage necessary to contain the data
fields has a lifetime equivalent to the scope in which it is declared.
No two record variables share the same data. It is not necessary to call
``new`` to create a record.

By contrast, a class variable contains only a reference to a class
instance. A class instance is created through a call to its ``new``
operator. Storage for a class instance, including storage for the data
associated with the fields in the class, is allocated and reclaimed
separately from variables referencing that instance. The same class
instance can be referenced by multiple class variables.

.. _Assignment_Differences:

Assignment
~~~~~~~~~~

Assignment to a class variable is performed by reference, whereas
assignment to a record is performed by value. When a variable of class
type is assigned to another variable of class type, they both become
names for the same object. In contrast, when a record variable is
assigned to another record variable, then contents of the source record
are copied into the target record field-by-field.

When a variable of class type is assigned to a record, matching fields
(matched by name) are copied from the class instance into the
corresponding record fields. Subsequent changes to the fields in the
target record have no effect upon the class instance.

Assignment of a record to a class variable is not permitted.

.. _Argument_Differences:

Arguments
~~~~~~~~~

Record arguments use the ``const ref`` intent by default - in contrast
to class arguments which pass by ``const in`` intent by default.

Similarly, the ``this`` receiver argument is passed by ``const in`` by
default for class methods. In contrast, it is passed by ``ref`` or
``const ref`` by default for record methods.

No *nil* Value
~~~~~~~~~~~~~~

Records do not provide a counterpart of the ``nil`` value. A variable of
record type is associated with storage throughout its lifetime, so
``nil`` has no meaning with respect to records.

.. _Record_Delete_Illegal:

The *delete* operator
~~~~~~~~~~~~~~~~~~~~~

Calling ``delete`` on a record is illegal.

.. _Comparison_Operator_Differences:

Default Comparison Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For records, the compiler will supply default comparison operators if
they are not supplied by the user. In contrast, the user cannot redefine
``==`` and ``!=`` for classes. The default comparison operators for a
record examine the arguments’ fields, while the comparison operators for
classes check whether the l.h.s. and r.h.s. refer to the same class
instance or are both ``nil``.
