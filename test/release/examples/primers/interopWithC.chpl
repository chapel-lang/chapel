// Interoperability with C

/*
   By default, Chapel programs are compiled as the main program.  Sometimes the
   program is intended as a library whose contents can be called from other
   sources.  This primer will demonstrate how to call Chapel functions from C
   and how to use C symbols in Chapel.
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
//

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
   .. literalinclude:: cClient.c
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
   .. literalinclude:: cClient.c
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
   .. literalinclude:: cClient.c
      :language: C
      :lines: 8-9
*/

//
// After that, the exported functions may be called:
//

/*
   .. literalinclude:: cClient.c
      :language: C
      :lines: 11-13
*/

//
// When ending the C program, the user must explicitly shut down the Chapel
// runtime and module code.  This is done by calling ``chpl_library_finalize``:
//

/*
   .. literalinclude:: cClient.c
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

//
// C code can be used in Chapel by declaring the symbol as ``extern``:
//
extern proc baz(): c_int;

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

//
// Unlike ``export``, ``extern`` can also be applied to global variables,
// struct definitions, or even typedefs.
//
extern var x: int(32);
extern var y: uint(32) = 3;
extern type myType = c_int;
extern proc useMyType(arg: myType): int; // an extern function using the typedef

export proc callUseMyType() {
  var blah: myType = 17;
  writeln(useMyType(blah));
}

//
// TODO: structs, typedefs
//
// TODO: demonstrate "external name" syntax, too
//
// TODO: demonstrate including the code where the extern symbols are defined
//       e.g. require statements or providing them on the compile line
//
// TODO: talk about types that are already available without extern definitions?
//       e.g. c_int, etc.
//

//
// TODO:
// - passing arrays back and forth
// - finish using C from Chapel
