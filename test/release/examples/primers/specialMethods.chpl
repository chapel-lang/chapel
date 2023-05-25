// Special Methods

/* This primer covers special methods for classes and records */

// Classes and records can have a handful of methods that are specially named
// and treated differently than other methods.  These methods enable
// operations such as initializing or deinitializing a class or record
// instance, accessing a class or record as though it were an array,
// iterating over an class or record, and defining the way a class or record
// is read from or written to a channel.
//
// Note that there are two ways to declare a method. Methods declared within
// a class or record are called primary methods:

record ExampleRecord1 {
  var exampleField: int;
  proc primaryMethod() { }
}


// Methods declared outside of a class or record are called secondary methods:

record ExampleRecord2 {
  var exampleField: int;
}
proc ExampleRecord2.secondaryMethod() { }

// Methods declared outside of a class or record and outside of the scope where
// the type is defined are called tertiary methods.  Tertiary methods follow the
// same declaration syntax as secondary methods.

// This primer will use the secondary method form, but all of the special
// methods can also be written as primary methods.

// First we will declare a simple record with a field that is a tuple of
// integers.  We'll add special methods and iterators to this record later.
record R {
  param size: int = 10;
  var vals: size*int;
}

/*
  Initializers and Deinitializers
  -------------------------------
*/

// An initializer named ``init`` is called when creating an instance of the
// class or record, for example with the ``new`` keyword. An initializer
// accepting zero arguments is called a *default initializer*.

// A method named ``init=`` is called a *copy initializer* and accepts a
// single argument.

// If a method named ``postinit`` that accepts zero arguments exists for a
// class or record type, it will automatically be called after the
// initializer call completes.

// The ``deinit`` method will deinitialize a record when it leaves scope,
// or a class when ``delete`` is called on it. If the class or record
// contained any unmanaged classes, open files, etc. this method would be
// the place to delete them or otherwise clean them up. See the :doc:`records`
// primer for more details on initializers and deinitializers.

/*
  The ``this`` Accessor
  ---------------------
*/

// The ``this`` method gives the record the ability to be accessed like an
// array.  Here we use the argument as an index to choose a tuple element.
proc R.this(n: int) ref {
  if !vals.indices.contains(n) then
    halt("index out of bounds accessing R");
  return vals[n];
}


// All functions and methods in Chapel can be called using either parenthesis
// or square brackets. Here we'll access the record by implicitly calling
// the ``this`` method defined above.

var r = new R();

r[0] = 1;
r(2) = 3;

writeln(r.vals);

/*
  Default Iterators
  -----------------
*/

// An iterator named ``these`` that can accept zero arguments is automatically
// called when a record or class instance is used in the iterator position
// of a ``for`` loop.
iter R.these() ref {
  for i in vals.indices {
    yield vals[i];
  }
}

for val in r {
  val += 1;
}
writeln(r.vals);

// Classes and records can also define parallel iterators including
// leader/follower iterator pairs and standalone parallel iterators. For
// more information on parallel iterators, see the :doc:`parIters` primer.

/*
  Custom Hashing
  --------------
*/

// By default, the compiler will define a hash method for any record
// that does not define its own ``==`` or ``!=`` overloads.  This
// permits such records to be used as the indices of associative
// domains, the values in a :mod:`Set`, or the keys in a :mod:`Map`.
// Users can override this default by supplying their own hash method
// that returns a ``uint`` or ``int`` value.  For example:

use Map;

proc R.hash(): int {
  writeln("In custom hash function");
  return vals[0];
}

// Now that the record R has a ``hash`` method defined, Chapel's,
// ``set``, ``map``, and associative domain types will call this
// custom ``hash`` instead of the compiler-generated method.

var myMap = new map(R, int);
var myD: domain(R);
var myR = new R();

myMap[myR] = 5;
myD += myR;

/*
  IO Methods
  ----------
*/

// The ``writeThis`` method defines how to write an instance of R to a
// channel. We'll write the ``vals`` tuple between asterisks. See section
// :ref:`readThis-writeThis` for more information  on the ``writeThis`` and
// ``readThis`` methods.

use IO; // required for file operations

config const filename = "tempfile.txt";

proc R.writeThis(ch: fileWriter) throws {
  ch.write("*", vals, "*");
}

{
  // Open the file in a new block so that deinitializers
  // will close it at the end of the block
  var f = open(filename, ioMode.cw);
  var ch = f.writer();
  ch.writeln(r);
}

// The ``readThis`` method defines how to read an instance of R from a
// channel. We'll read the ``vals`` tuple between asterisks like how it
// was written above.
proc R.readThis(ch: fileReader) throws {
  var star = new ioLiteral("*");
  ch.read(star);
  ch.read(vals);
  ch.read(star);
}

{
  var f = open(filename, ioMode.r);
  var ch = f.reader();
  var r2 = new R();
  ch.readln(r2);
  assert(r == r2);
}

{
  var chW = openWriter(filename);
  chW.writeln(r);
  chW.flush();

  writeln(r);
  var r2 = new R();
  var chR = openReader(filename);
  chR.readln(r2);
  assert(r == r2);
}

// Clean up the temporary file we created earlier.
{
  use FileSystem;
  if exists(filename) then
    remove(filename);
}

/*
  Operator Overloads
  ------------------
*/

// Operators can be overloaded for record types to support
// assignment (``=``), comparisons, (``<``, ``<=``, ``>``, ``>=``, ``==``,
// ``!=``), and other general operators (``+``, ``-``, ``*``, ``/``, ...).
// These are declared as regular functions using the ``operator`` keyword.
