.. _readme-extern:

.. default-domain:: chpl

==================
C Interoperability
==================

This README describes support in the Chapel compiler for referring to C
code within Chapel using a keyword named `extern`. These features are
still in the process of being improved.

Note that it is also possible to call Chapel from C using the `export`
keyword. Please see :ref:`readme-libraries` for details.

External C functions, variables, and types can be referred to within a
Chapel program. The section `Working with C`_ below describes the
basic ideas of how Chapel and C interoperate. There are two supported
strategies for providing the Chapel compiler with information about the
C declarations that should be usable from Chapel code.

 1) The explicit strategy is to use extern declarations, as described in
    the section `Support for Extern Declarations`_ below.  When using the
    explicit strategy, one creates a Chapel declarations for each C
    function, variable, or type that one wants to use from Chapel code.
    This explicit strategy allows for a great deal of manual control over
    how the Chapel compiler views a C function or type. This strategy is
    typically used within the Chapel standard modules for portability
    reasons. The :ref:`c2chapel` tool can be used to automatically generate
    extern declarations from a valid C99 file.

 2) The extern block feature provides an implicit strategy, as described in
    the section `Support for Extern Blocks`_ below. This strategy makes
    use of the `clang` parser and so requires a Chapel compiler built with
    LLVM support. The main advantage of the extern block strategy is that
    it is not necessary to provide Chapel declarations for every desired C
    function or type. Instead, such declarations are added automatically.
    In addition, the extern block can contain C function definitions in
    addition to declarations and so provides a way in which programs that
    need to be written in both C and Chapel can be implemented in a single
    source file.

Working with C
==============

Whether using extern declarations or extern blocks, it is important
to understand the basic interoperability support provided by the
Chapel compiler.

The general approach is as follows:

1) Programmers inform the Chapel compiler about C concepts that they want
   to refer to in their Chapel code with :ref:`explicit extern
   declarations <readme-extern-extern-declarations>` or by :ref:`creating
   extern declarations with extern blocks <readme-extern-extern-block>`.
2) Programmers use the extern C concepts that have been described. That
   might include declaring variables, calling functions, or operating on
   already declared global variables. See `Using Extern Declarations`_.
3) Library and header dependencies are described on the Chapel compiler's
   command line or with ``require`` statements as described in the section
   `Expressing Dependencies`_.

The next section describes the basic information about how the Chapel
compiler views common C types.


.. _readme-extern-standard-c-types:

Standard C Types
================

In declaring extern C procedures and variables, one of the
requirements is to accurately describe their types within the Chapel
'extern' declarations.  Because the C specification allows compilers
to determine how many bits are used in the representation of various
types, one must be careful to avoid false assumptions, such as that a
C 'int' can always be represented via a Chapel 'int' or 'int(32)'.

To help with mapping types between C and Chapel, Chapel installations
contain a standard module named :mod:`CTypes` (located in
``$CHPL_HOME/modules/standard/``).  This module defines a number of
type aliases that accurately describe C types using their Chapel
equivalents.  Most of these are prefixed by ``c_`` to distinguish them
from Chapel type names that may have different meanings.

The Chapel names for C types are:

.. code-block:: chapel

  c_int
  c_uint
  c_long
  c_ulong
  c_longlong
  c_ulonglong
  c_char
  c_schar
  c_uchar
  c_short
  c_ushort
  ssize_t
  size_t
  c_void_ptr
  c_ptr(T)
  c_ptrConst(T)
  c_array(T,n)
  c_string

For consistency, the following type aliases are also provided even
though their sizes can't vary in C (thereby permitting the equivalent
Chapel types to always be usable):

.. code-block:: chapel

  c_float  // (a real(32) in Chapel)
  c_double // (a real(64) in Chapel)

c_void_ptr, c_string, c_ptr(T), c_ptrConst(T), and c_array(T,n) are
described in the next section.


Pointer and String Types
------------------------

Chapel supports the following C pointer types: c_void_ptr, c_ptr(T),
c_ptrConst(T), c_string, and c_fn_ptr. In addition, it supports c_array(T,n).

These types are the same as C types:

.. code-block:: text

  c_void_ptr is void*
  c_ptr(T) is T*
  c_ptrConst(T) is const T*
  c_string is const char*
  c_fn_ptr represents a C function pointer (with unspecified arg and return types)
  c_array(T,n) is T[n]

Note that in some cases, a ref argument intent may be used in place of
c_void_ptr or c_ptr(T), and const ref intent in place of c_ptrConst(T).

These pointer types may only point to local memory. The intent is
that they will be used to interoperate with C libraries that run within a
single locale. In addition, these pointer types must be treated carefully as it
is possible to create the same kinds of problems as in C - in particular, it is
necessary know that one of these types points to valid memory before it is
used.  As in C, these pointers could point to a stack variable that is no
longer valid, or to memory that has been freed. The Chapel language makes no
effort to extend a variable's lifetime if it is converted in some manner to a C
pointer.

c_void_ptr
~~~~~~~~~~

The c_void_ptr type is provided as an opaque C pointer. Since the type is
unknown, there is no way to dereference this pointer. In addition, it is not
possible to construct a c_void_ptr directly in Chapel. Normally, a C function
will return the void pointer, which will be passed to other C functions.

c_ptr(T)
~~~~~~~~

The c_ptr(T) type is a generic type representing a C pointer to an arbitrary
type T. This pointer should normally only point to local memory - since no
communication will be generated when it is dereferenced.  Of course, the
pointed-to type T should be one that is supported in C interoperability if the
c_ptr(T) is used for C interoperability. The c_ptr(T) type supports
indexing to get a reference to the i'th element (starting from 0).

c_ptrConst(T)
~~~~~~~~~~~~~

The c_ptrConst(T) type is equivalent to c_ptr(T), except it disallows changing
the pointed-to value. Like C, this does not change anything about the pointee's
inherent mutability; it is simply not possible to mutate anything via a const
pointer. It is also possible to use a c_ptr(T) where a c_ptrConst(T) is called
for, but not vice-versa without explicitly casting away the constness.

c_array(T,n)
~~~~~~~~~~~~

The c_array(T,n) type is a generic value type representing a C fixed-size
array. Here 'n' is the number of elements and must be known at compile-time.

The c_array type is a value type in Chapel code but it can coerce to
a c_ptr(T) type.

Allocating a variable of c_array type in a function will allocate that
variable on the stack. Indexing into a c_array works similarly to
indexing into a c_ptr and starts from 0. c_array supports by-value copy
initialization and assignment.

.. _readme-extern-standard-c-types-ref-intents:

ref intents
~~~~~~~~~~~

Note that when declaring extern procedures that use function arguments that are
passed by pointer in C, it is recommended to use the ref argument intent
instead of c_ptr(T). Using the ref intent allows the arguments to the extern
proc to be passed directly instead of needing to be converted to a C pointer
first. For example, both the functions byRef and byPtr below have the same C
prototype, but they must be used differently in Chapel:

.. code-block:: chapel

  // both of these correspond to void fn(int* x)
  extern proc byRef(ref x:c_int);
  extern proc byPtr(x:c_ptr(c_int));

  var x:c_int = 7;
  byRef(x); // ref argument intent allows the variable to be passed directly
  byPtr(c_ptrTo(x)); // c_ptr argument must be constructed explicitly

Analogously, const ref may be used instead of c_ptrConst(T).


c_string
~~~~~~~~

The c_string type maps to a constant C string (that is, const char*)
that is intended for use locally. A c_string can be obtained from a
Chapel string using the method :proc:`~String.string.c_str`. A Chapel string can be
constructed from a C string using the cast operator. Note however that
because c_string is a local-only type, the .c_str() method can only be
called on Chapel strings that are stored on the same locale; calling
.c_str() on a non-local string will result in a runtime error.

.. note::

  ``c_string`` is expected to be deprecated in a future release in favor
  of instead using ``c_ptr`` types such as ``c_ptrConst(c_char)``.

c_fn_ptr
~~~~~~~~

The c_fn_ptr type is useful for representing arguments to external
functions that accept function pointers.  At present, there is no way
to specify the argument types or return type of the function pointer.
Chapel functions can be passed as arguments of type c_fn_ptr via the
c_ptrTo() call, as with other c_ptr types.  For example, given an
external C function foo() that takes in a pointer to a function that
accepts an int and returns a double, the following code would declare
that function and pass a Chapel function to it:

.. code-block:: chapel

  extern proc foo(f: c_fn_ptr);

  foo(c_ptrTo(bar));

  proc bar(x: c_int): c_double {
    ...
  }

Any calls that foo() makes through its function pointer argument will
call back to Chapel's bar() routine.  Note that any Chapel functions
passed as c_fn_ptr arguments cannot be overloaded nor generic.

.. _readme-extern-extern-declarations:

Support for Extern Declarations
===============================

Chapel allows users to refer to external C types, variables, and functions via
extern declarations. These external declarations are part of the Chapel
language and can be written alongside pure Chapel in any ".chpl" file. If
manually writing extern declarations isn't practical (e.g., for large
libraries), the :ref:`c2chapel` tool can be used to automatically generate
extern declarations.

Declaring External C Types
--------------------------

You can refer to other external C types using 'extern' plus the normal
type alias keyword.  By leaving off any sort of type definition, you
are telling the Chapel compiler that it can't assume it knows anything
about how the type is represented or how to operate on it (though the
compiler may make copies of values of the type using assignment and/or
memcpy()).

Effectively, such a declaration defines a new primitive type in
Chapel.  For example, the following declaration says that there is an
external type named 'foo':

.. code-block:: chapel

    extern type foo;

This permits you to declare variables of type 'foo' and to declare
external functions that accept or return arguments of type 'foo'.
Because Chapel knows nothing about the type 'foo' such variables
cannot be manipulated within Chapel apart from passing them between
routines.  The type symbol 'foo' must be declared in a C header file
provided on Chapel's compiler command-line.

If an external C type can be described in Chapel, that definition can
be given in Chapel, which permits the compiler to deal with the type
directly, as it would any other Chapel type alias.  For example, if
the external type 'foo' was a 64-bit integer, you could describe it in
Chapel using:

.. code-block:: chapel

    extern type foo = int(64);

Static, fixed-size C array types can be described within Chapel using
c_array. For example, the following C typedef:

.. code-block:: chapel

    typedef double vec[3];

could be described in Chapel using:

.. code-block:: chapel

    extern type vec = c_array(real(64),3);

To refer to more complex C types like external structs or pointers to
structs, see the section on `Declaring External C Structs`_ below.


Declaring External C Variables and Constants
--------------------------------------------

A C variable or constant can be referred to within Chapel by prefixing
its declaration with the extern keyword.  For example:

.. code-block:: chapel

    extern var bar: foo;

would tell the Chapel compiler about an external C variable named
'bar' of type 'foo'.  Similarly:

.. code-block:: chapel

   extern const baz: int(32);

would refer to an external 32-bit integer constant named 'baz' in the
C code.  In practice, external consts can be used to provide Chapel
definitions for #defines and enum symbols in addition to traditional C
constants.

Note that it is not currently possible to explicitly declare an `extern param`.
In the future, it might be possible to use an :ref:`extern block
<readme-extern-extern-block>` to import #define constants that are known at
compile time as param constants within Chapel.


Declaring External C Functions
------------------------------

To make a call to an external C function, you will need to prototype
the routine in your Chapel code using the ``extern`` keyword.  For
example, for a C function foo() that takes no arguments and returns
nothing, the prototype would appear as follows:

.. code-block:: chapel

       extern proc foo();

Declaring Return Types
~~~~~~~~~~~~~~~~~~~~~~


C functions that return values which you wish to refer to within your
Chapel program must have those return types declared. Note that the Chapel
compiler will not infer the return type as it does for Chapel functions.
To make the function above return a C "double", it would be declared:

.. code-block:: chapel

       extern proc foo(): real;

See the :ref:`readme-extern-declarations-limitations` section for
limitations on what types can be returned.

Declaring Arguments
~~~~~~~~~~~~~~~~~~~

Similarly, external functions that expect arguments must declare those
arguments in Chapel.

Types of function arguments may be omitted, in which case the types
will be inferred based on the Chapel callsite.  For example, the
following Chapel code:

.. code-block:: chapel

       extern proc foo(x: int, y): real;

       var a, b: int;

       foo(a, b);

Would imply that external function foo() is able to take two 32-bit
integer values and that it returns a 64-bit real ('double' in C).
External function declarations with omitted type arguments can be used
to support calls to external C macros or varargs functions that accept
multiple argument signatures.

.. _readme-extern-declarations-limitations:

Allowed Intents and Types
~~~~~~~~~~~~~~~~~~~~~~~~~

Since C passes and returns by value, a C argument such as ``int arg``
corresponds to an ``in`` intent argument in a Chapel ``extern proc``.
An argument such as ``int* ptrArg`` can be represented either with
``c_ptr(int)`` or with the ``ref`` intent in Chapel (and see
:ref:`readme-extern-standard-c-types-ref-intents` for a discussion of why
you would use one or the other). Correspondingly, ``const int* ptrArg`` can be
represented with ``c_ptrConst(int)`` or the ``const ref`` intent.

Note that, for numeric and pointer types, the default intent in Chapel is
already ``const in`` (see the spec section :ref:`Abstract_Intents`).

As of 1.23, there are several limitations on what types can be passed to
or returned from ``extern`` or ``export`` functions and what intents can
be used for the arguments to these functions.

First, here are the allowable argument and return types for ``extern``
and ``export`` functions:

 * Built-in numeric type and pointer types, including the C types
   described above

 * ``extern record`` types

 * ``string`` and ``bytes`` are allowed in an ``export proc`` but not in
   an ``extern proc``

   * see :ref:`readme-libraries`

 * array types are allowed in some cases

   * ``extern proc`` arguments currently allow single-locale rectangular
     arrays in which case the argument will be a pointer to the first
     element
   * ``export proc`` s can support more Chapel array types - see
     :ref:`readme-libraries`

 * ``unmanaged`` and ``borrowed`` class types are allowed


The following types are not allowed as argument or return types for
``extern`` or ``export`` functions:

  * ``owned`` and ``shared`` classes
  * any Chapel record type that is not an ``extern record``

Only the following argument intents are allowed in ``extern`` and
``export`` functions:

  * for built-in numeric and pointer types - default intent and ``const``
    (these correspond to ``const in`` for those types)
  * ``in``
  * ``const in``
  * ``ref``
  * ``const ref``

Additionally, ``type`` intent arguments are allowed for ``extern``
functions (see :ref:`readme-extern-declarations-type-arguments`).

Varargs Functions
~~~~~~~~~~~~~~~~~

Default arguments can be declared for external function arguments, in
which case the Chapel compiler will supply the default argument value
if it is omitted at the callsite.  For example:

.. code-block:: chapel

       extern proc foo(x: int, y = 1.2): real;

       foo(0);

Would cause external function foo() to be called with the arguments 0
and 1.2.

C varargs functions can be declared using Chapel's varargs ("...")
syntax.  For example, the following declaration prototypes C's printf
function:

.. code-block:: chapel

       extern proc printf(fmt: c_string, vals...?numvals): int;

Note that it can also be prototyped more trivially/less accurately
as follows:

.. code-block:: chapel

       extern proc printf(args...): int;

which relies on the callsite to pass in reasonable arguments
(otherwise, the C compilation step will likely fail).

.. _readme-extern-declarations-type-arguments:

Type Arguments
~~~~~~~~~~~~~~

External C functions or macros that accept type arguments can also be
prototyped in Chapel by declaring the argument as a type.  For
example:

.. code-block:: chapel

       extern foo(type t);

Calling such a routine with a Chapel type will cause the type
identifier (e.g., 'int') to be passed to the routine.  In practice,
this will typically only be useful if the external function is a macro
or built-in (like sizeof()) that can handle type identifiers.

Array Arguments
~~~~~~~~~~~~~~~

Extern functions with array arguments are handled as a special case within the
compiler. As an example:

.. code-block:: chapel

       extern proc foo(x: [] int, n: int);

This procedure definition will match up to a C function with the prototype of

.. code-block:: chapel

       void foo(int64_t* x, int64_t n);

The Chapel compiler will then rewrite any calls to `foo` like this:

.. code-block:: chapel

      foo(x, 10); // -> foo(c_ptrTo(x), 10);

Note that this same technique won't work for distributed rectangular arrays,
nor for associative, sparse, or opaque arrays because their data isn't
necessarily stored using a representation that translates trivially to a C
array.  The compiler will automatically insert ``use CTypes;`` into scopes
containing an ``extern proc`` declaration with an array argument in order
to support the pointer types used to pass the array to the external routine.

Renaming Extern Procs
~~~~~~~~~~~~~~~~~~~~~

It is possible to provide the Chapel compiler with a different
name for the function than the name available to other Chapel code.
For example, if there is a C function called ``foo_in_c`` returning an int,
we might use the following declaration to provide that function to
other Chapel code with the name ``foo_in_chapel``

.. code-block:: chapel

       extern "foo_in_c" proc foo_in_chapel(): c_int;
       writeln(foo_in_chapel()); // will generate a call to foo_in_c

An arbitrary expression can be used instead of the string literal
`"foo_in_c"` from this example. The expression just needs to evaluate
to a `param` `string`.

Frequently Asked Questions About Declaring External Routines
------------------------------------------------------------

How do I pass a Chapel variable to an external routine that expects
a pointer?

Today, the preferred way to do this is to declare the argument as
having 'ref' intent.  This should cause the Chapel compiler to pass
a pointer to the argument.  For example, given a C function:

.. code-block:: chapel

  void foo(double* x);

This could be called in Chapel using:

.. code-block:: chapel

  extern proc foo(ref x: real);

  var myVal: real = 1.2;

  foo(myVal);


Declaring External C Structs
----------------------------

External C struct types can be referred to within Chapel by prefixing
a Chapel record definition with the extern keyword.  For example,
given an external C structure defined in foo.h called fltdbl:

.. code-block:: c

    typedef struct _fltdbl {
      float x;
      double y;
    } fltdbl;

This type could be referred to within a Chapel program using:

.. code-block:: chapel

   extern record fltdbl {
   }

Within the Chapel declaration, some or all of the fields from the C
structure may be specified.  Any fields that are not specified (or
that cannot be specified because there is no equivalent Chapel type)
cannot be referenced within the Chapel code.  The order of these
fields need not match the order they were specified within the C code.
As an example, the following declaration would permit access to both
fields x and y within variables of type fltdbl:

.. code-block:: chapel

   extern record fltdbl {
     var x: real(32);
     var y: real(64);
   }

as would the following declaration:

.. code-block:: chapel

   extern record fltdbl {
     var y: real(64);
     var x: real(32);
   }

Alternatively, the external declaration could only mention one of the
fields.  For example, the following declaration would permit field y
to be accessed by a Chapel program, but not field x (though both would
still be stored in any variable of type 'fltdbl').

.. code-block:: chapel

   extern record fltdbl {
     var y: real(64);
   }

Alternatively, the external declaration can avoid mentioning any
fields, which would permit a variable of that struct type to be passed
between Chapel and C routines, but without permitting its fields to be
accessed within the Chapel program:

.. code-block:: chapel

   extern record fltdbl {
   }


Extern records can work with pointers or fixed-sized arrays.
Suppose we have this C structure:

.. code-block:: c

    typedef struct bufptr {
      int buf[16];
      int* ptr;
    } bufptr;

It can be declared in Chapel as follows:

.. code-block:: chapel

   extern record bufptr {
     var buf:c_array(c_int, 16);
     var ptr:c_ptr(c_int);
   }


A C header file containing the struct's definition in C must be
specified on the chpl compiler command line or in a ``require`` statement
as described in `Expressing Dependencies`_.

To work with a C structure that is not typedef'd, just name the
C type name after the ``extern`` keyword. In the example below,
the ``struct stat`` type does not have a corresponding typedef
in C. Therefore, we inform the Chapel compiler that the C name
for the type is ``struct stat``:

.. code-block:: chapel

  extern "struct stat" record chpl_stat_type {
    var st_size: off_t;
  }

  proc getFileSize(path:c_string) : int {
    extern proc stat(x: c_string, ref buf:chpl_stat_type): c_int;
    var buf: chpl_stat_type;

    if (chpl_stat_function(path, buf) == 0) {
      return buf.st_size;
    }
    return -1;
  }

  writeln(getFileSize("stat-example.chpl"));


Opaque Types
------------

NOTE: This support may eventually be deprecated as other 'extern'
features become increasingly flexible and robust.

You can refer to other external pointer-based C types that cannot be
described in Chapel using the "opaque" keyword.  As the name implies,
these types are opaque as far as Chapel is concerned and cannot be
used for operations other than argument passing and assignment
(to/from other similarly opaque types).

For example, Chapel could be used to call an external C function that
returns a pointer to a structure (that we can't or won't describe as
an external class using the previous mechanism) as follows:

.. code-block:: chapel

    extern proc returnStructPtr(): opaque;

    var structPtr: opaque = returnStructPtr();

However, because structPtr's type is opaque, it cannot be used for
much apart from assigning it to other opaque variables of matching
underlying type, or passing it back to an external C routine that
expects a pointer-to-struct of that type:

.. code-block:: chapel

    extern proc operateOnStructPtr(ptr: opaque);

    var copyOfStructPtr = structPtr;

    operateOnStructPtr(structPtr);


.. _readme-extern-extern-block:

Support for Extern Blocks
=========================

[Note: The features in this section rely on Chapel to being built with
llvm/clang enabled.  To do so, set environment variable CHPL_LLVM to
'bundled' and rebuild your Chapel installation. See :ref:`readme-llvm`.].

C code and header files can be included directly within Chapel source
code using an ``extern block`` as follows:

.. code-block:: chapel

  extern {
    #include "my_C_API.h"
    static int myCInt = 7;
    ....
  }

Such ``extern { }`` block statements add the top-level C statements to
the enclosing Chapel module.  This is similar to what one might do
manually using the extern declarations (as described above), but can
save a lot of labor for a large API.  Moreover, using an inline extern
block permits you to write C declarations directly within Chapel
without having to create distinct C files.

An ``extern { }`` also adds ``private use`` statements for the following
modules since they will generally be used by the implicitly generated
``extern proc`` functions:

 * :mod:`CTypes`

If you don't want to have a lot of C symbols cluttering up a module's
namespace, it's easy to put the C code into its own Chapel module:

.. code-block:: chapel

  module C {
    extern {
      static int foo(int x) { return x + 1; }
      ... c code here...
    }
  }

  writeln(C.foo(3));

In that event, you might consider adding ``public use CTypes;``
to the module so that the types and functions generated by the extern
block will be usable.

This feature strives to support C global variables, functions, structures,
typedefs, enums, and some #defines. Structures always generate a Chapel record,
and pointers to a structure are represented with c_ptr(struct type). Also,
pointer arguments to functions are always represented with c_ptr or c_string
instead of the ref intent.

Note that functions or variables declared within an extern block should either
be declared in a .h file that is #included or they should be be declared
``static`` (otherwise you might get a warning during C compilation).

Also note that a ``static inline`` function can be inlined into Chapel
code that calls it. Thus, by using ``static inline`` in an ``extern block``,
it is possible to hand-tune a computational kernel by writing some of
it in inline assembly.

#defines
--------

The extern block functionality allows simple #defines to be used from
Chapel code. Simple defines take no arguments and define an integer,
string, or real value or use another #define that does so. Casts to
built-in numeric types are also supported, as are macros that are simply
an alternative name for another C variable. For example:

.. code-block:: chapel

  extern {
   #define NEG_ONE (-1)
   #define MY_NUMBER ((unsigned int)NEG_ONE)
  }
  writeln(NEG_ONE);
  writeln(MY_NUMBER);

However, it is easy to create #defines that are not supported because the
Chapel compiler is unable to determine their expression type. In particular,
any #define taking arguments is not currently supported. For example, the
ADD definition below has a resulting expression type that cannot be determined
by the Chapel compiler:

.. code-block:: chapel

 extern {
   #define ADD(x,y) ((x)+(y)) // NOT SUPPORTED
 }
 var x = ADD(1,2); // ERROR - ADD not defined in Chapel.

If the library you are using depends on these types, the current solution
is to create inline functions to call the define with the appropriate argument
types; for example:

.. code-block:: chapel

 extern {
   #define ADD(x,y) ((x)+(y))
   static inline int ADD_int_int(int x, int y) { return ADD(x,y); }
 }
 var sum = ADD_int_int(1,2);
 writeln(sum);


Pointer Types
-------------

See the section `Pointer and String Types`_ above for background on
how the Chapel programs can work with C pointer types. Any pointer type used in
an extern block will be made visible to the Chapel program as c_ptr(T),
c_ptrConst(T) (for const pointer types besides char) or c_string
(for const char* types).

For example:

.. code-block:: chapel

 extern {
   static void setItToOne(int* x) { *x = 1; }
   // will translate automatically into
   //  extern proc setItToOne(x:c_ptr(c_int));

   static void getItPlusOne(const int* x) { return *x + 1; }
   // will translate automatically into
   //  extern proc getItPlusOne(x:c_ptrConst(c_int));

   // The Chapel compiler can't know if X is used as an array,
   // if the argument will come from a Chapel variable, and in more general
   // cases, the best way to handle multiple levels of pointers. For example:
   static void setSpace(int** x) {
     static int space[10];
     *x = space;
   }
   // translates automatically into
   //  extern proc returnSpace( x:c_ptr(c_ptr(c_int)) );

   static void setString(const char** x) { *x = "My String"; }
 }
 var x:c_int;
 setItToOne(c_ptrTo(x));

 var y:c_int = 5
 writeln(getItPlusOne(c_ptrToConst(y))); // could also just use c_ptrTo(y)

 var space:c_ptr(c_int);
 setSpace(c_ptrTo(space));

 var str:c_string;
 setString(c_ptrTo(str));
 writeln(toString(str));

As you can see in this example, using the extern block might result in
more calls to c_ptrTo() when using the generated extern declarations,
because the compiler cannot automatically distinguish between several
common cases (passing an array vs. passing in an argument by reference).

Example
-------

Here's a more complete example:

.. code-block:: chapel

  module MyCModule {
    extern {
      static int foo(int b) { return b + 1; }
    }
  }

  writeln("Hello");
  writeln(MyCModule.foo(7));

This prints out

::

  Hello
  8

which demonstrates full integration between Chapel and the C function
it calls.


Using Extern Declarations
=========================

Extern declarations can be used just like normal Chapel types, variables,
or functions. Using these extern declarations from Chapel code requires
some care.

Parallel Safety
---------------

If external routines are to be called from Chapel within parallel
execution contexts, they should be parallel-safe.  As with internal Chapel
routines, it is also the responsibility of the Chapel programmer to call
external routines in a manner that preserves the integrity of objects
accessible to those routines.  Simply put, objects shared across Chapel
tasks must be kept parallel-safe within Chapel.

Multiple Locales
----------------

Since the extern C code does not generally have any support for multiple
locales, it is important to take care when using this code from multiple
locales. Here are some things to be aware of:

 * extern global variables will refer to a local version of that variable
   on each locale. These variables might become different if they
   are changed differently on different locales.

 * c_ptr is always generated as a narrow pointer type (in other words, it
   does not encode the locale storing the pointed-to value - just an
   address). That means that passing a c_ptr from one locale to another
   and then using it on the second locale will probably result in a core
   dump.

Initialization
--------------

Chapel variables of extern type are not generally initialized
automatically. Be sure to manually initialize Chapel variables of extern
type.

The Chapel compiler assume that extern records can be copied to each
other without running any copy initializer. Similarly, it does not call
any deinitializer for extern records.

In the future, we would like to support automatic zero initialization
of such variables and a way to provide their default initializer.

Working with c_ptr
------------------

A c_ptr type can be constructed from a Chapel variable using the c_ptrTo()
function; for example:

.. code-block:: chapel

 var i:c_int;
 var i_ptr = c_ptrTo(i); // now i_ptr has type c_ptr(c_int) == int* in C

Similarly, a c_ptrConst can be constructed using c_ptrToConst():

.. code-block:: chapel

 var i:c_int; // i could also be 'const'
 var i_ptrConst = c_ptrToConst(i); // now i_ptrConst has type c_ptrConst(c_int) == const int* in C

Since a C pointer might refer to a single variable or an array, the c_ptr type
supports 0-based array indexing and dereferencing. In addition, it is possible
to allocate and free space for one or more elements and return the result as a
c_ptr. See the following example:

.. code-block:: chapel

  var cArray = c_calloc(c_int, 10);
  for i in 0..#10 {
   cArray[i] = i:c_int;
  }
  // c_ptr.deref() always refers to the first element.
  cArray.deref() = 17;
  for i in 0..#10 {
   writeln(cArray[i]);
  }
  c_free(cArray);

Variables of type c_ptr can be compared against or set to nil.


Working with strings
--------------------

If you need to call a C function and provide a Chapel string, you may need to
convert the Chapel string to a C string first.  Chapel string literals will
automatically convert to C strings.  A Chapel string variable can be converted
using the :proc:`~String.string.c_str` method.

myprint.h:

.. code-block:: c

  void myprint(const char* str);

myprint.c:

.. code-block:: c

  void myprint(const char* str) {
    printf("%s\n", str);
  }

myprint.chpl:

.. code-block:: chapel

  extern proc myprint(str:c_string);

  // string literal is automatically converted to a c_string
  myprint("hello");

  // a string variable must be converted with .c_str()
  var s = "goodbye";
  myprint(s.c_str());


Expressing Dependencies
=======================

Any required C header files, source code files, object files, or
library files must be provided to the Chapel compiler by one of
two mechanisms.

 1) They can be listed at compile-time on the Chapel command line For
    example, if an external function foo() was defined in foo.h and foo.c,
    it could be added to the compilation using any of these commands:

    .. code-block:: sh

       chpl foo.h foo.c myProgram.chpl
       chpl foo.h foo.o myProgram.chpl #if foo.c had already been compiled)
       chpl foo.h -lfoo myProgram.chpl #if foo.c had been archived in libfoo.a)

    Note that you can use -I and -L arguments for the Chapel compiler
    to specify include or library paths as with a C compiler.

 2) Alternatively, the required C resources can be listed within the
    Chapel file using the `require` statement. For example:

    .. code-block:: chapel

       require "foo.h", "foo.c";

    This has an effect similar to adding foo.h and foo.c to the Chapel
    compiler's command line. Filenames are interpreted as expressing a
    path relative to the directory in which the source file lives.
    You can also use the compiler's -I and -L flags to indicate search
    directories for headers or library files.

    Similarly, the version below uses the require statement to indicate
    that this module depends on libfoo.a (and has a similar effect as if
    ``-lfoo`` were added to the command line).

    .. code-block:: chapel

       require "foo.h", "-lfoo";

    Require statements accept general ``param`` string expressions
    beyond the string literals shown in these examples.  Only
    ``require`` statements in code that the compiler considers
    executable will be processed.  Thus, a ``require`` statement
    guarded by a ``param`` conditional that the compiler folds out, or
    in a module that does not appear in the program's ``use``
    statements will not be added to the program's requirements.  For
    example, the following code either requires ``foo.h`` or whatever
    requirement is specified by *defaultHeader* (``bar.h`` by default)
    depending on the value of *requireFoo*:

    .. code-block:: chapel

       config param requireFoo=true,
                    defaultHeader="bar.h";

       if requireFoo then
         require "foo.h";
       else
         require defaultHeader;


Either approach has the following results:

 * During Chapel's C code generation stage, any header files listed on the
   compiler's command line or in a require statement will be #include'd by
   the generated code in order to ensure that the appropriate prototypes
   are found before making any references to the external symbols.
 * During Chapel's C compilation stage, any C files on the command line or
   in a require statement will be compiled using the same flags as the
   Chapel-generated C files (use --print-commands to see these compile
   commands).
 * During Chapel's link step, any .o and .a files listed on the compiler's
   command-line or in require statements will be included in the final
   executable.


Future Directions
=================

We intend to continue improving these capabilities to provide richer
support for external types and functions over time.  If you
have specific requests for improvement, please let us know on the
`Chapel GitHub issues page`_ or community forums.

.. _Chapel GitHub issues page: https://github.com/chapel-lang/chapel/issues
