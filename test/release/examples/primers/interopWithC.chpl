// Interoperability with C

/*
   By default, Chapel programs are compiled as the main program.  Sometimes the
   program is intended as a library whose contents can be called from other
   sources.  This primer will demonstrate how to call Chapel functions from C
   and how to call C functions from Chapel.
*/

/*
   .. _primers-C-interop-functions:

   Symbol Availability
   -------------------

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
   .. _primers-C-interop-using-C:

   Using C code
   ------------
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

extern var x: int(32);
extern var y: uint(32) = 3;

//
// TODO: demonstrate "external name" syntax, too
//
// TODO: demonstrate including the code where the extern symbols are defined
//       e.g. require statements or providing them on the compile line
//
// TODO: talk about types that are already available without extern definitions?
//       e.g. c_int, etc.
//

/*
  .. _primers-C-interop-compiling:

  Compiling as a Library
  ----------------------
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
// TODO:
// - calling Chapel from C
// - passing arrays back and forth
// - importing the module
// - how to link to Chapel
// - maybe calling C from Chapel?
//
// Need to copy the C client code to the docs directory, but then can write
// stuff like:
//
/*
   .. literalinclude:: cClient.c
      :language: C
      :lines: 1-17
*/
