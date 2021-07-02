// Interoperability with C

/*
   By default, Chapel programs are compiled as the main program.  Sometimes the
   program is intended as a library whose contents can be called from other
   sources.  This primer will demonstrate how to call Chapel functions from C
   as well as how to use C symbols in Chapel.
*/

/*
   .. _primers-C-interop-Chapel-as-library:

   Chapel as a Library
   -------------------

   .. _primers-C-interop-functions:

   Symbol Availability
   +++++++++++++++++++

   (TODO: better name, prolly)
*/

//
// When creating a library file from Chapel code, only those symbols with
// ``export`` attached to them will be available from outside the library.
//

export proc foo() { // foo will be available to outside code
  writeln("Called foo()");
}

proc bar() { // bar will not be available to outside code
  writeln("Called bar()");
}

//
// At present, only functions can be exported from Chapel code.
//

/*
   .. _primers-C-interop-compiling-library:

   Compiling the Chapel Library
   ++++++++++++++++++++++++++++
*/

//
// To generate a library from a Chapel code file, compile the Chapel file with
// the ``--library`` flag.  E.g.
//
/*
   .. code-block:: bash

      chpl --library interopWithC.chpl
*/

//
// There are various flags and settings that can be used to modify the library
// that gets created and create additional helper files.  These are listed in
// detail in the :ref:`readme-libraries` Technical Note.  For this primer, we
// will focus on the default settings and ``--static`` compilation - this will
// rely on the generated library being named ``libinteropWithC.a``, with a
// generated header file named ``interopWithC.h``, both of which live in a
// generated ``lib`` directory.
// Since ``--static`` is not supported on MacOS you will have to dynamically link
// the generated library. Please refer to :ref:`readme-libraries` for instructions
// on how to do that.

/*
   .. _primers-C-interop-using-library:

   Using the Chapel Library
   ++++++++++++++++++++++++
*/

//
// To access the symbols in the generated Chapel library from a C program,
// ``#include`` the generated header file:
//

/*
   .. literalinclude:: cClient.test.c
      :language: C
      :lines: 1-2
*/

//
// Before calling any of the functions defined by the library, the Chapel
// runtime and internal libraries must be initialized.  This is done by calling
// ``chpl_library_init``, which is defined in
// ``$CHPL_HOME/runtime/include/chpl-init.h`` and accessible with the above
// ``#include``.
//

/*
   .. literalinclude:: cClient.test.c
      :language: C
      :lines: 5-7
*/

//
// Then, if the exported functions rely upon any global variables, the module
// initialization function declared in the header file must be called.  If there
// is uncertainty about whether doing so is necessary, it is recommended to call
// this function.  In this example, this call occurs like this:
//

/*
   .. literalinclude:: cClient.test.c
      :language: C
      :lines: 8-9
*/

//
// After that, the exported functions may be called:
//

/*
   .. literalinclude:: cClien.test.c
      :language: C
      :lines: 11-13
*/

//
// When ending the C program, the user must explicitly shut down the Chapel
// runtime and module code.  This is done by calling ``chpl_library_finalize``:
//

/*
   .. literalinclude:: cClient.test.c
      :language: C
      :lines: 15-16
*/

//
// Once shut down, the Chapel runtime cannot be restarted in the current C
// program.
//

/*
   .. _primers-C-interop-compiling-2:

   Compiling C Code with the Chapel Library
   ++++++++++++++++++++++++++++++++++++++++
*/

//
// Compiling C code with the generated Chapel library file is generally complex,
// but can be made simpler through the use of the ``--library-makefile`` flag
// as described in :ref:`readme-libraries-linking`.
//
// An example of compiling a C program with a generated Chapel library using the
// generated Makefile can be found in the `Makefile
// <https://github.com/chapel-lang/chapel/blob/master/test/release/examples/primers/Makefile>`_
// for the primers directory, to build this source file.
//

/*
   .. _primers-C-interop-using-C:

   Using C Code in Chapel
   ----------------------
*/
// Chapel has support for C code but we need to tell the compiler
// about the function using the ``extern`` keyword

extern proc baz(): int;

//
// The function can then be called as normal:
//
proc callsBaz() {
  writeln(baz());
}

//
// Such functions can even be called in exported functions:
//

export proc alsoCallsBaz() {
  writeln(baz());
  callsBaz();
}

// You can tell the chapel compiler where to look for these C functions by adding a
// require statement
require "cHelper.h", "cHelper.c";
// You must inlcude both a header file and C file.
// Chapel also supports require statements for ``.o`` files and
// for archived libraries using the ``-l`` flag
/*
   .. code-block:: chapel

         require "foo.o", "-lapache-arrow";
*/

// Alternatively you can also include their names while invoking the chapel compiler.

//
// Unlike ``export``, ``extern`` can also be applied to global variables,
// struct definitions, or even typedefs.
//
extern var x: int(32);
extern var y: uint(32) = 3;
extern type myType = int;
extern proc useMyType(arg: myType): int; // an extern function using the typedef

export proc callUseMyType() {
  var blah: myType = 17;
  writeln(useMyType(blah));
}

// Chapel also has a standard module named :mod:`SysCTypes` (located under
//``$CHPL_HOME/modules/standard/gen/...``) which defines a few C types which
// align with the C compiler specification and do not require the extern keyword
// such as ``c_int`` and ``c_char``. For more information about these types see
// the :ref:`readme-extern` Technical Note.

// You can include SysCTypes using a simple use statement

use SysCTypes;


// You can also assign default arguments to extern procs which then can be omitted
// at the callsite as usual. For example

extern proc sum(a: c_int, b: c_int = 1): int;

// We must always make sure the types align as the C compiler specification allows for
// different sizes for the same type depending on the compiler.

// In order to pass an array to C, we use the following declaration:
extern proc sumArray(arr: [] int, size: c_int): c_int;
//Where an arrya would be
var arr : [0..9] int = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
// Keep in mind that since Chapel has 64 bit ints, the C function must also accept an
// of type ``int64_t``

//
// A C struct can be used in Chapel by declaring it as an ``extern record``.
//

extern record rec {
  var a: c_int;
}

extern proc giveRec(): rec; // returns an instance of the struct

// You do not have to inform chapel about all the fields of a record,
// only the ones that you wish to directly manipulate using Chapel code.

// For example a record with no declared fields is possible even though
// the actualy C struct might have a nonzero number of fields.

extern record notReallyEmpty{
}

// This means that the type is just reduced to being able to be passed around to
// other functions and without the ability to directly manipulate it.

// As of now the struct must be defined completely in the included header file
// and must also be typdef'd.
// In order to include a struct which is not typedef'd or if you want to import it
// under another name simple state its external name after the ``extern`` keyword

extern "struct person" record person{
   var name: c_string;
   var age: c_int;
}

// ``extern proc``s can also be renamed in a similar fashion.

// Since most functions dealing with structs often return pointers, you can use the
// ref intent for function arguments when their C counterparts are dealing with 
// pointers

require "fact.c", "fact.h";

extern record data{
    var x: c_int;
}

extern proc getNewData() ref : data;

ref d :data;
d = getNewData();

extern proc fact_d(ref x: data) ref : data;
ref f : data;
f = fact_d(d);


// If you do not care about the type for a certain variable or argument, you can
// use the ``opaque`` keyowrd to indicate to the compiler that you do not know about
// the type.
// Such a variable will not be much use except for the ability to pass it to different routines
// which accept the same underlying type. (Be carefull here as it may lead to unmatched types)

extern proc getDataStructPtr(): opaque;
var structPtr: opaque = getDataStructPtr();

// For the ability to use C code in Chapel without an external C file,
// You can also use extern blocks which allow you to put C code directly into Chapel files
// To avoid cluttering you namespace you can also put these inside a module

module CDemo {
   extern {
      #include<math.h>

      static double square(double num){
         return pow(num,2);
      }
   }
}
writeln(CDemo.square(3));
