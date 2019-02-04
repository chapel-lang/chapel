// Special Methods

/* This primer covers special methods for classes and records */

// Classes and records can have a handful of methods that are specially named
// and treated differently than other methods.  These methods enable
// operations such as initializing or deinitializing a class or record
// instance, accessing a class or record as though it were an array,
// iterating over an class or record, and defining the way a class or record
// is read from or written to a channel.
//
// Throughout this primer, the methods are defined as secondary methods
// using the form: ``proc TypeName.methodName() ...``
//
// It is also possible to declare these methods within the body of the
// class or record as primary methods: ``proc methodName() ...``

// First we will declare a simple record with a field that is a tuple of
// integers.  We'll add special methods and iterators to this record later.
record R {
  param size: int;
  var vals: size*int;
}

/*
  Initializers and Deinitializers
  -------------------------------
*/

// An initializer is called when creating an instance of the class or record,
// for example with the ``new`` keyword. Here we initialize the record with
// the given size.
proc R.init(param size: int = 10) {
  writeln("Initialized an R instance");
  this.size = size;
}

var r = new R();

// The ``deinit`` method will deinitialize the record when it leaves scope.
// If the record contained any unmanaged classes, open files, etc. this
// method would be the place to delete them or otherwise clean them up.
proc R.deinit() {
  writeln("Deinitialized an R instance");
}

/*
  The ``this`` Accessor
  ---------------------
*/

// The ``this`` method gives the record the ability to be accessed like an
// array.  Here we use the index to choose a tuple element.
proc R.this(n: int) ref {
  if n < 1 || n > size then
    halt("index out of bounds accessing R");
  return vals[n];
}

r[1] = 1;
r[3] = 3;

writeln(r.vals);

/*
  Default Iterators
  -----------------
*/

// An iterator named ``these`` is automatically called when a record or class
// instance is used in the iterator position of a ``for`` loop.
iter R.these() ref {
  for i in 1..size {
    yield vals[i];
  }
}

for val in r {
  val += 1;
}
writeln(r.vals);

// Classes and records can also define parallel iterators.  Here we define
// a standalone parallel iterator. This iterator cannot be zippered with
// other iterators like leader/follower iterators, but can be simpler
// to write and faster to execute since it doesn't need to conform to
// the expectations of other iterators.
//
// For more information on parallel iterators, see the :doc:`parIters` primer.
iter R.these(param tag: iterKind) ref where tag == iterKind.standalone {
  use RangeChunk;
  const nTasks = 4;

  coforall tid in 0..#nTasks {
    var myChunk = chunk(1..size, nTasks, tid);

    for i in myChunk {
      yield vals[i];
    }
  }
}

forall val in r {
  val += 1;
}
writeln(r.vals);

/*
  IO Methods
  ----------
*/


// Define how to write an instance of R to a channel. We'll write the
// 'vals' tuple between asterisks. See section
// :ref:`readThis-writeThis-readWriteThis` for more information  on the
// ``writeThis``, ``readThis``, and ``readWriteThis`` methods.
proc R.writeThis(ch: channel) {
  ch.write("*", vals, "*");
}

{
  // Open the file in a new block so that deinitializers
  // will close it at the end of the block
  var f = open("tempfile.txt", iomode.cw);
  var ch = f.writer();
  ch.writeln(r);
}

// Define how to read an instance of R from a channel. We'll read the
// 'vals' tuple between asterisks.
proc R.readThis(ch: channel) {
  var star = new ioLiteral("*");
  ch.read(star);
  ch.read(vals);
  ch.read(star);
}

{
  var f = open("tempfile.txt", iomode.r);
  var ch = f.reader();
  var r2 = new R();
  ch.readln(r2);
  writeln("r:  ", r);
  writeln("r2: ", r2);
  assert(r == r2);
}

// If the record should be read and written using the same
// format, the combined 'readWriteThis' method can replace the
// 'readThis' and 'writeThis' methods. This method will be
// used for both reading and writing the 'vals' tuple
// surrounded by double asterisks. The ``readThis`` and
// ``writeThis`` methods defined above have higher precedence
// than ``readWriteThis``, so this function is not used because
// they are defined.
proc R.readWriteThis(ch: channel) {
  const star = new ioLiteral("**");
  ch <~> star <~> vals <~> star;
}

{
  var chW = openwriter("tempfile.txt");
  chW.writeln(r);
  chW.flush();

  writeln(r);
  var r2 = new R();
  var chR = openreader("tempfile.txt");
  chR.readln(r2);
  assert(r == r2);
  
}
