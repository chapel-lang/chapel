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
// array.  Here we use the the argument as an index to choose a tuple element.
proc R.this(n: int) ref {
  if n < 1 || n > size then
    halt("index out of bounds accessing R");
  return vals[n];
}


// All functions and methods in Chapel can be called using either parenthesis
// or square brackets. Here we'll access the record by implicitly calling
// the ``this`` method defined above.

var r = new R();

r[1] = 1;
r(3) = 3;

writeln(r.vals);

/*
  Default Iterators
  -----------------
*/

// An iterator named ``these`` that can accept zero arguments is automatically
// called when a record or class instance is used in the iterator position
// of a ``for`` loop.
iter R.these() ref {
  for i in 1..size {
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
  IO Methods
  ----------
*/

// The ``writeThis`` method defines how to write an instance of R to a
// channel. We'll write the ``vals`` tuple between asterisks. See section
// :ref:`readThis-writeThis-readWriteThis` for more information  on the
// ``writeThis``, ``readThis``, and ``readWriteThis`` methods.

use IO; // required for file operations

config const filename = "tempfile.txt";

proc R.writeThis(ch: channel) throws {
  ch.write("*", vals, "*");
}

{
  // Open the file in a new block so that deinitializers
  // will close it at the end of the block
  var f = open(filename, iomode.cw);
  var ch = f.writer();
  ch.writeln(r);
}

// The ``readThis`` method defines how to read an instance of R from a
// channel. We'll read the ``vals`` tuple between asterisks like how it
// was written above.
proc R.readThis(ch: channel) throws {
  var star = new ioLiteral("*");
  ch.read(star);
  ch.read(vals);
  ch.read(star);
}

{
  var f = open(filename, iomode.r);
  var ch = f.reader();
  var r2 = new R();
  ch.readln(r2);
  assert(r == r2);
}

// If the record should be read and written using the same
// format, the combined ``readWriteThis`` method can replace the
// ``readThis`` and ``writeThis`` methods. This method will be
// used for both reading and writing the ``vals`` tuple
// surrounded by double asterisks. The ``readThis`` and
// ``writeThis`` methods defined above have higher precedence
// than ``readWriteThis``, so this function is not used because
// they are defined.
proc R.readWriteThis(ch: channel) throws {
  const stars = new ioLiteral("**");
  ch <~> stars <~> vals <~> stars;
}

{
  var chW = openwriter(filename);
  chW.writeln(r);
  chW.flush();

  writeln(r);
  var r2 = new R();
  var chR = openreader(filename);
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
// These are declared as regular functions with two arguments, not as methods
// on the record.
