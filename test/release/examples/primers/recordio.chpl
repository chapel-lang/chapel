/* This example demonstrates how to use formatted I/O to
   write and read a record stored as tab-separated
   fields, with one record per line.
 */

/* MyRecord contains fields of various types in order to demonstrate
   functionality.
 */
use IO;  // enable access to the features in this primer

record MyRecord : serializable {
  var i: int;
  var r: real;
  var s: string;
}

// configs to control the output location and whether debugging is enabled
config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, ioMode.cwr);


// Let's create a few records and store them in an array.
var A = [ new MyRecord(1,3.0,"testone"),
          new MyRecord(6,-1.1,"quickbrown"),
          new MyRecord(9,1e6,"fox") ];

// We'll read back into B and check that they match...
var B: [0..#3] MyRecord;

// Create a writer that we'll use to write the data.
{
  var writer = f.writer(locking=false);

  // Now let's write the records in a particular format:
  // 1 line per record
  // each field separated by a tab
  for a in A {
    // Depending on the situation, you might do:
    // ``writer.writeln(a.i, "\t", a.r, "\t", a.s);``
    // that is equivalent to this (%? means any type):
    // ``writer.writef("%?\t%?\t%?\n", a.i, a.r, a.s);``
    // but if you wanted to control precision/width and to
    // handle strings with tabs, you might use:
    writer.writef("%2i\t%2.2r\t%s\n", a.i, a.r, a.s);
    // (%'S asks for a single-quoted string)

    // for debugging purposes, we also output it to stdout
    if debug then writef("%2i\t%2.2r\t%s\n", a.i, a.r, a.s);
  }

  writer.close();
}

// Now read the data - Way 1: use formatted I/O
{
  var reader = f.reader(locking=false);

  var rec:MyRecord;
  var i = 0;

  // clear B (only for our testing)
  if debug then B = rec;

  // read until we reach EOF
  // (note: if you want to handle format errors or I/O errors,
  //  you need to use error= versions of the I/O functions)
  while( reader.readf("%?\t%?\t%s\n", rec.i, rec.r, rec.s) ) {
    // for debugging purposes, we also output it to stdout
    if debug then writeln("read ", rec);
    B[i] = rec;
    assert( rec == A[i] );
    i += 1;
  }

  // Check that A == B.
  assert( A == B );

  reader.close();
}

// Now read the data - Way 2: provide serialize/deserialize methods.
// Note that this didn't work with Chapel 1.31 or earlier.

/* notes on serialize/deserialize (see :ref:`serialize-deserialize`):

   - reader is a fileReader, writer is a fileWriter

   - the compiler will generate default serialize/deserialize methods for you if
     you don't provide one

 */
proc ref MyRecord.deserialize(reader, ref deserializer) throws {
  i = reader.read(int);
  reader.readLiteral("\t");
  r = reader.read(real);
  reader.readLiteral("\t");
  s = reader.read(string);
  reader.readLiteral("\n");
}

proc MyRecord.serialize(writer, ref serializer) throws {
  writer.write(i);
  writer.writeLiteral("\t");
  writer.write(r);
  writer.writeLiteral("\t");
  writer.write(s);
  writer.writeLiteral("\n");
}

proc MyRecord.init(i: int = 0, r: real = 0.0, s: string = "") {
  this.i = i;
  this.r = r;
  this.s = s;
}

proc MyRecord.init(r: fileReader(?), ref deserializer: ?dt) throws {
  this.init();
  deserialize(r, deserializer);
}

{
  var reader = f.reader(locking=false);

  var rec:MyRecord;
  var i = 0;

  // clear B (only for our testing)
  B = rec;

  // read until we reach EOF
  // (note: if you want to handle format errors or I/O errors,
  //  you need to use error= versions of the I/O functions)
  while( reader.read(rec) ) {
    if debug then writeln("read ", rec);
    B[i] = rec;
    assert( rec == A[i] );
    i += 1;
  }

  // Check that A == B.
  assert( A == B );

  reader.close();
}
