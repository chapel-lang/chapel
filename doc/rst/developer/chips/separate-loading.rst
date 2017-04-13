Dynamically Loading Chapel Code
===============================

Status
  Draft

Author
  Michael Ferguson


Abstract
--------

Rationale
---------

As of Chapel 1.15, there are several issues preventing any straightforward
way to separately create and then load Chapel code at run-time.

Such a technique is related to several other concepts:

 * separately compiling and then statically linking a Chapel program
 * interpreting Chapel programs

TODO: why dynamic loading?

Imagine loading code in to a running Chapel program on multiple
locales.

Imagine a continuous development environment between
compiled and optimized code and "interpreted" code.

Imagine flexibility of allowing specialization to apply only
when necessary for performance.

Imagine a natural way to detect functions that need not
be instantiated.


Challenges
----------

In this section, we identify the issues that prevent straightforward
separate compilation and dynamic loading of Chapel code at run-time.

Generic Functions
+++++++++++++++++

Today, the Chapel compiler handles generic functions and types by
specializing the code for whatever types they are used with. For example,
a function accepting a single generic argument will be "stamped out" for
each possible type of argument that it can be called with. (This
strategy is known as template specialization in C++ or monomorphization
in Rust).

As with C++, this strategy presents problems for complete separate
compilation. In C++, one of two things needs to happen in order to
use a generic function supplied by a library:

 1) the generic function must be implemented in a header available
    to the user of the library, or
 2) the compile-time parameters (usually types) needed to specialize
    the generic function match those for a specialization of the generic
    function supplied by the library.

It is difficult to call either of these strategies "separate
compilation".

Ftable, Class IDs and Virtual Method Table
++++++++++++++++++++++++++++++++++++++++++

 - could be done with a custom linker or by
   generating and dl'opening something

    - do we have to re-visit existing decisions?
    - e.g. class ID for an existing class could change!

    -> better probably to not change class IDs of existing
       data on a load

    -> new class IDs are added, extend existing virtual
       method table, etc
       
    -> new FTable entries are added by extending existing table

 -> runtime managing ftable array and virtual methods table
    array (and extending them) should be sufficient

 -> even OK to use Schubert numbering if depth-first traversal
    is stored in a separate array (not assigning class IDs)

Separating Generic Functions
----------------------------

Background
++++++++++

Recall that Chapel and C++ use a strategy of specializing generic
functions at compile time in order to "stamp out" instantiations
appropriate for particular types. This strategy presents a challenge
for creating separate shared libraries.

The Java generics system offers a different approach. Instead of
specializing each generic function, code is generated to represent the
function in its generic form. This strategy is also known as type
erasure. It effectively amounts to putting off until run-time some of the
determinations of which functions to invoke for which calls. This shares
some similarities with virtual methods.  (in Chapel terms, it does some
function resolution at run-time).

The type erasure strategy used in Java has issues as well:

 * it is slower than the specialization approach
 * the type arguments must always be classes (not primitive types like
   int)

See http://faq.sealedabstract.com/rust/ which indicates

 * Rust is guaranteed to specialize generics
 * in Swift this is an implementation detail
 * Rust and Swift both contain generic code in some form (byte code) in
   libraries
 * Swift has the ability to totally avoid specialization

See https://github.com/apple/swift/blob/master/docs/Generics.rst
for more information about Swift's generics. In particular, that document
lists as a goal:

  * "Generic code should be compiled such that it can be executed with
    any data type without requiring a separate "instantiation" step"

In order to understand the compile-time performance implications
of some of these questions, I created a program that creates hundreds
of tuple types and measured the compile-time behavior across Swift, C++,
and Chapel.

============ ============ ============ ============
n tuples     Swift        Chapel       C++
============ ============ ============ ============
100          0.15s 80KB    23s 8MB     7.8s 3MB
200          0.15s 80KB    45s 6MB     17s 8MB
400          0.16s 80KB    97s 11MB    40s 19MB
2000         0.17s 90KB
25000        0.17s 91KB
50000        0.19s 106KB
============ ============ ============ ============


Idea
++++

What if the Chapel compiler arranged to perform function resolution at
run-time? And an optimization pass was available to complete the function
resolution process at compile time?


What would that look like?

 * types would have a run-time representation and function arguments
   would be extended to include a type argument to represent the type
   of other arguments
 * local variables and array elements would have sizes computed at run-time
 * params would exist at run-time
 * run-time structures would exist to perform function resolution.
   Since resolution needs to perform overload resolution, these
   structures would effectively implement multiple dispatch.
   Additionally, these structures would need to contain an idea
   of scopes.

Let's consider an example.

.. code-block:: chapel

  module Test {
    proc f(x)
    where isIntegerType(x.type) // note: isInteger type returns param
    {
      for i in 1..2 {
        // note: cast to string is a call to cast(type string, x);
        writeln(x:string);
      }
    }
    proc f(x:string) {
      writeln(x);
    }
  }

How can both versions of `f` be compiled without specialization?
Here is a straw-man for generated code to do it.

.. code-block:: C

  // These would be provided by the "runtime"

  // type_t represents a type at run-time
  typedef struct {
    size_t size;
    type_info_t* info;
  } type_t;

  // type_or_param_arg_t represents a type or param arg at run-time
  // it has a fixed and small maximum size.
  typedef struct {
    union {
      type_t  type;
      int64_t p_int;
      double  p_real;
      ...
    } u;
  } type_or_param_t;

  // empty_param is a global type_or_param representing no param
  // param_type, type_type, void_type, string_type are global built-in types

  // Chooses which function to call based on type/param arguments.
  // Chooses between overloads. The resulting function might still
  // be generic, but this funtion must compute its return type.
  void resolveChapelFn(// in
                       const char* name,
                       type_t arg1_type, type_or_param_t* arg1,
                       type_t arg2_type, type_or_param_t* arg2,
                       ...,
                       // out
                       fn_ptr_t* fn,
                       type_t* return_type,
                       type_or_param_t* returned_type_or_param
                      );

  void callChapelFn(// in
                    fn_ptr_t* fn,
                    type_t arg1_type, void* arg1, 
                    type_t arg2_type, void* arg2, 
                    ...,
                    // out: sets *return_slot
                    void*  return_slot
                   );

  // this is a sketch of the code generated by the compiler for module Test
  void f1(// in: args and types
          type_t x_type, void* x,
          // out: during resolution: return type, returned param
          type_t* return_type_out, type_or_param_t* returned_type_or_param_out,
          // out: during execution: returned value
          void* return_slot) {

    // in the start of the generated function, handle
    //  resolving any called functions
    //  stack allocating any local variables
    // this is to prevent redundant work & to avoid alloca in a loop
    fn_ptr_t _cast = NULL;
    type_t _cast_return_type = {0};
    fn_ptr_t writeln = NULL;
    type_t writeln_return_type = {0};
    type_or_param_t string_type_arg = { {.type = string_type} };

    // resolve _cast(type string, x)
    resolveChapelFn("_cast", // name
                    type_type, &string_type_arg, // types & params
                    x_type, empty_param,
                    &_cast, &_cast_return_type, NULL // out: fn, return type
                   );
    // resolve writeln
    resolveChapelFn("writeln", // name
                    _cast_return_type, NULL, // types & params
                    &writeln, &writeln_return_type, NULL // out: fn, return type
                   );
 
    // after resolution, the return type can always be known
    if (return_type_out != NULL) {
      *return_type_out = void_type;
      return;
    }

    // to prepare to call _cast, create stack space for the result of _cast
    void* cast_result = alloca(_cast_return_type.size);

    // to prepare to call writeln, create stack space for the result of writeln
    void* writeln_result = alloca(writeln_result.size);


    // now comes the main computation in the function
    // (loop would almost certainly be generated differently in reality;
    //  it is here for pedagogical reasons)
    for (int i = 1; i <= 2; i++ ) {
    
      // now actually call the cast fn
      callChapelFn(_cast, // which fn to call
                   type_type, &string_type, x_type, x, // types & args
                   cast_result // out: return slot
                  );

      // now actually call the writeln fn
      callChapelFn(writeln, // which fn to call
                   _cast_return_type, cast_result, // types & args
                   writeln_result // out: return slot
                  );
    }

    // would set return_slot if f1 returned
  }
  bool f1_where(type_t x_type, void* x) {
    type_or_param_t x_type_arg = { {.type = x_type} };
    fn_ptr_t isIntegerType = NULL;
    type_t isIntegerType_return_type = {0};
    type_or_param_t isIntegerType_returned_type_or_param = {0};
    resolveChapelFn("isIntegerType", // name
                    type_type, &x_type_arg, // types & params
                    // out: fn, return type, returned type or param
                    &isIntegerType,
                    &isIntegerType_return_type,
                    &isIntegerType_returned_type_or_param
                   );
    if (isIntegerType_return_type == return_param &&
        isIntegerType_returned_type_or_param.u.bool == true)
      return true;
    else
      return false;
  }
  // may need to generate other f1 helper functions to enable
  // determination of f1's return type.

  void f2(void* x, type_t x_type) {
    // could assert x_type == string_type
    
    fn_ptr_t writeln = NULL;
    type_t writeln_return_type = {0};
    type_or_param_t string_type_arg = { {.type = string_type} };

    // resolve writeln
    resolveChapelFn("writeln", // name
                    string_type, NULL, // types & params
                    &writeln, &writeln_return_type, NULL // out: fn, return type
                   );
 
    void* writeln_result = alloca(writeln_result.size);

    // now actually call the writeln fn
    callChapelFn(writeln, // which fn to call
                 string_type, x, // types & args
                 writeln_result // out: return slot
                );
  }

  // to be called when dynamically loading this module
  void initTest() {
    registerChapelFn("Test", "f", f1, f1_where, any_type);
    registerChapelFn("Test", "f", f2, NULL,     string_type);
  }


Further wrinkles:

 - the return intent overload feature would require resolveChapelFn
   to return a set of possibilities. These would be selected
   in a second phase of resolution. (Including ref-if-modified
   arguments).

    * resolveChapelFn could set const-ness of some arguments
    * "resolution" section of function implements ref-if-modified
      and choosing return intent overloads
    * "resolution" section may need to contain two sub-phases

        1) resolve functions called
        2) resolve const-ness of ref-if-modified arguments
           and choose between return intent overloads

Partial Evaluation
------------------

The first Futamura projection is when an interpreter is specialized for a
particular source code.




APPENDIX
--------

.. code-block:: swift

  func f<T>( _ arg:T ) {
    let mirror = Mirror(reflecting:arg)
    let count = mirror.children.count
    print("count", count)
    for child in mirror.children {
      print("label", child.label ?? "_", "value", child.value)
    }
    print(arg)
  }

  func g<T>( _ i: Int, _ arg: T) {
    let t1 = (i, arg)
    f(t1)
    let t2 = (i, i, arg, arg)
    f(t2)
    let t3 = (i, i, i, arg, arg, arg)
    f(t3)
    let t4 = (i, i, i, i, arg, arg, arg, arg)
    f(t4)
    let t5 = (i, i, i, i, i, arg, arg, arg, arg, arg)
    f(t5)
    let t6 = (i, i, i, i, i, i, arg, arg, arg, arg, arg, arg)
    f(t6)
    let t7 = (i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg)
    f(t7)
    let t8 = (i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg, arg)
    f(t8)
    let t9 = (i, i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg, arg, arg)
    f(t9)
    let t10 = (i, i, i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg, arg, arg, arg)
    f(t10)
  }

  func h<T>( _ i: Int, _ arg: T) {
    let t1 = (arg,i)
    g(i, t1)
    let t2 = (arg, arg, i, i)
    g(i, t2)
    let t3 = (arg, arg, arg, i, i, i)
    g(i, t3)
    let t4 = (arg, arg, arg, arg, i, i, i, i)
    g(i, t4)
    let t5 = (arg, arg, arg, arg, arg, i, i, i, i, i)
    g(i, t5)
    let t6 = (arg, arg, arg, arg, arg, arg, i, i, i, i, i, i)
    g(i, t6)
    let t7 = (arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i)
    g(i, t7)
    let t8 = (arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i, i)
    g(i, t8)
    let t9 = (arg, arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i, i, i)
    g(i, t9)
    let t10 = (arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i, i, i, i)
    g(i, t10)
  }


  h( 1, (0.0,1) )
  h( 2, (0.0,1,2) )
  h( 3, (0.0,1,2,3) )
  h( 4, (0.0,1,2,3,4) )
  h( 5, (0.0,1,2,3,4,5) )
  h( 6, (0.0,1,2,3,4,5,6) )
  h( 7, (0.0,1,2,3,4,5,6,7) )
  h( 8, (0.0,1,2,3,4,5,6,7,8) )
  h( 9, (0.0,1,2,3,4,5,6,7,8,9) )
  h(10, (0.0,1,2,3,4,5,6,7,8,9,10) )
  h(11, (0.0,1,2,3,4,5,6,7,8,9,10,11) )
  h(12, (0.0,1,2,3,4,5,6,7,8,9,10,11,12) )
  h(13, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13) )
  h(14, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14) )
  h(15, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15) )
  h(16, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) )
  h(17, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17) )
  h(18, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18) )
  h(19, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) )
  h(20, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20) )
  h(21, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21) )
  h(22, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22) )
  h(23, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23) )
  h(24, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24) )
  h(25, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25) )

  h(26, (0.0,1.0) )
  h(27, (0.0,1.0,2) )
  h(28, (0.0,1.0,2,3) )
  h(29, (0.0,1.0,2,3,4) )
  h(30, (0.0,1.0,2,3,4,5) )
  h(31, (0.0,1.0,2,3,4,5,6) )
  h(32, (0.0,1.0,2,3,4,5,6,7) )
  h(33, (0.0,1.0,2,3,4,5,6,7,8) )
  h(34, (0.0,1.0,2,3,4,5,6,7,8,9) )
  h(35, (0.0,1.0,2,3,4,5,6,7,8,9,10) )
  h(36, (0.0,1.0,2,3,4,5,6,7,8,9,10,11) )
  h(37, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12) )
  h(38, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13) )
  h(39, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14) )
  h(40, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15) )
  h(41, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) )
  h(42, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17) )
  h(43, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18) )
  h(44, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) )
  h(45, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20) )
  h(46, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21) )
  h(47, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22) )
  h(48, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23) )
  h(49, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24) )
  h(50, (0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25) )

  print("done")


.. code-block:: c++

  #include <iostream>
  #include <tuple>

  template<typename T>
  void f( T arg ) {
    int count = std::tuple_size<T>::value;
    std::cout << "count " << count << "\n";
    std::cout << std::get<0>(arg) << "\n";
  }

  template<typename T>
  void g( int i, T arg) {
    auto t1 = std::make_tuple(i, arg);
    f(t1);
    auto t2 = std::make_tuple(i, i, arg, arg);
    f(t2);
    auto t3 = std::make_tuple(i, i, i, arg, arg, arg);
    f(t3);
    auto t4 = std::make_tuple(i, i, i, i, arg, arg, arg, arg);
    f(t4);
    auto t5 = std::make_tuple(i, i, i, i, i, arg, arg, arg, arg, arg);
    f(t5);
    auto t6 = std::make_tuple(i, i, i, i, i, i, arg, arg, arg, arg, arg, arg);
    f(t6);
    auto t7 = std::make_tuple(i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg);
    f(t7);
    auto t8 = std::make_tuple(i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg,
        arg, arg, arg);
    f(t8);
    auto t9 = std::make_tuple(i, i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg,
        arg, arg, arg, arg);
    f(t9);
    auto t10 = std::make_tuple(i, i, i, i, i, i, i, i, i, i, arg, arg, arg, arg,
        arg, arg, arg, arg, arg, arg);
    f(t10);
  }

  template<typename T>
  void h( int i, T arg) {
    auto t1 = std::make_tuple(arg,i);
    g(i, t1);
    auto t2 = std::make_tuple(arg, arg, i, i);
    g(i, t2);
    auto t3 = std::make_tuple(arg, arg, arg, i, i, i);
    g(i, t3);
    auto t4 = std::make_tuple(arg, arg, arg, arg, i, i, i, i);
    g(i, t4);
    auto t5 = std::make_tuple(arg, arg, arg, arg, arg, i, i, i, i, i);
    g(i, t5);
    auto t6 = std::make_tuple(arg, arg, arg, arg, arg, arg, i, i, i, i, i, i);
    g(i, t6);
    auto t7 = std::make_tuple(arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i,
        i);
    g(i, t7);
    auto t8 = std::make_tuple(arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i,
        i, i, i, i);
    g(i, t8);
    auto t9 = std::make_tuple(arg, arg, arg, arg, arg, arg, arg, arg, arg, i, i,
        i, i, i, i, i, i, i);
    g(i, t9);
    auto t10 = std::make_tuple(arg, arg, arg, arg, arg, arg, arg, arg, arg, arg,
        i, i, i, i, i, i, i, i, i, i);
    g(i, t10);
  }


  int main() {

  h( 1, std::make_tuple(0.0,1) );
  h( 2, std::make_tuple(0.0,1,2) );
  h( 3, std::make_tuple(0.0,1,2,3) );
  h( 4, std::make_tuple(0.0,1,2,3,4) );
  /* not much point in going beyond 4*100 at the moment
  h( 5, std::make_tuple(0.0,1,2,3,4,5) );
  h( 6, std::make_tuple(0.0,1,2,3,4,5,6) );
  h( 7, std::make_tuple(0.0,1,2,3,4,5,6,7) );
  h( 8, std::make_tuple(0.0,1,2,3,4,5,6,7,8) );
  h( 9, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9) );
  h(10, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10) );
  h(11, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11) );
  h(12, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12) );
  h(13, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13) );
  h(14, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14) );
  h(15, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15) );
  h(16, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) );
  h(17, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17) );
  h(18, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18) );
  h(19, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) );
  h(20, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20) );
  h(21, std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21)
  );
  h(22,
  std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22) );
  h(23,
  std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23)
  );
  h(24,
  std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24)
  );
  h(25,
  std::make_tuple(0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25)
  );
  ;
  h(26, std::make_tuple(0.0,1.0) );
  h(27, std::make_tuple(0.0,1.0,2) );
  h(28, std::make_tuple(0.0,1.0,2,3) );
  h(29, std::make_tuple(0.0,1.0,2,3,4) );
  h(30, std::make_tuple(0.0,1.0,2,3,4,5) );
  h(31, std::make_tuple(0.0,1.0,2,3,4,5,6) );
  h(32, std::make_tuple(0.0,1.0,2,3,4,5,6,7) );
  h(33, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8) );
  h(34, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9) );
  h(35, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10) );
  h(36, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11) );
  h(37, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12) );
  h(38, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13) );
  h(39, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14) );
  h(40, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15) );
  h(41, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) );
  h(42, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17) );
  h(43, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18) );
  h(44, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) );
  h(45, std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)
  );
  h(46,
  std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21) );
  h(47,
  std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22)
  );
  h(48,
  std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23)
  );
  h(49,
  std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24)
  );
  h(50,
  std::make_tuple(0.0,1.0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25)
  );
  */

  std::cout << "done\n";
  return 0;
  }


.. code-block:: chapel

  proc f( arg ) {
    writeln("count ", arg.size);
    for i in 1..arg.size {
      writeln("label .", i, " value ", arg);
    }
    writeln(arg);
  }

  proc g( i:int, arg ) {
    const t1 = (i, arg);
    f(t1);
    const t2 = (i, i, arg, arg);
    f(t2);
    const t3 = (i, i, i, arg, arg, arg);
    f(t3);
    const t4 = (i, i, i, i, arg, arg, arg, arg);
    f(t4);
    const t5 = (i, i, i, i, i, arg, arg, arg, arg, arg);
    f(t5);
    const t6 = (i, i, i, i, i, i, arg, arg, arg, arg, arg, arg);
    f(t6);
    const t7 = (i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg);
    f(t7);
    const t8 = (i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg, arg);
    f(t8);
    const t9 = (i, i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg, arg,
        arg);
    f(t9);
    const t10 = (i, i, i, i, i, i, i, i, i, i, arg, arg, arg, arg, arg, arg, arg,
        arg, arg, arg);
    f(t10);
  }

  proc h( i:int, arg ) {
    const t1 = (arg,i);
    g(i, t1);
    const t2 = (arg, arg, i, i);
    g(i, t2);
    const t3 = (arg, arg, arg, i, i, i);
    g(i, t3);
    const t4 = (arg, arg, arg, arg, i, i, i, i);
    g(i, t4);
    const t5 = (arg, arg, arg, arg, arg, i, i, i, i, i);
    g(i, t5);
    const t6 = (arg, arg, arg, arg, arg, arg, i, i, i, i, i, i);
    g(i, t6);
    const t7 = (arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i);
    g(i, t7);
    const t8 = (arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i, i);
    g(i, t8);
    const t9 = (arg, arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i, i, i,
        i);
    g(i, t9);
    const t10 = (arg, arg, arg, arg, arg, arg, arg, arg, arg, arg, i, i, i, i, i, i,
        i, i, i, i);
    g(i, t10);
  }

  h( 1, (0.0,1) );
  h( 2, (0.0,1,2) );
  h( 3, (0.0,1,2,3) );
  h( 4, (0.0,1,2,3,4) );
  /* not much point in going beyond 4*100 at the moment
  h( 5, (0.0,1,2,3,4,5) );
  h( 6, (0.0,1,2,3,4,5,6) );
  h( 7, (0.0,1,2,3,4,5,6,7) );
  h( 8, (0.0,1,2,3,4,5,6,7,8) );
  h( 9, (0.0,1,2,3,4,5,6,7,8,9) );
  h(10, (0.0,1,2,3,4,5,6,7,8,9,10) );
  h(11, (0.0,1,2,3,4,5,6,7,8,9,10,11) );
  h(12, (0.0,1,2,3,4,5,6,7,8,9,10,11,12) );
  h(13, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13) );
  h(14, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14) );
  h(15, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15) );
  h(16, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16) );
  h(17, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17) );
  h(18, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18) );
  h(19, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) );
  h(20, (0.0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20) );
  */
  writeln("done");

