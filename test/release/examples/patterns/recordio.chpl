/* This example demonstrates how to use formatted I/O to
   write and read a record stored as a tab-separated
   fields, with one record per line.
 */

/* MyRecord contains fields of various types in order to demonstrate
   functionality.
 */
use IO;

record MyRecord {
  var i: int;
  var r: real;
  var s: string;
}

config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, iomode.cwr);


// Let's create a few records and store them in an array.
var A = [ new MyRecord(1,3.0,"test one"),
          new MyRecord(6,-1.1,"quick brown"),
          new MyRecord(9,1e6,"fox") ];

// We'll read back into B and check that they match...
var B: [0..#3] MyRecord;

{
  // Create a writer that we'll use to write the data.
  var writer = f.writer();

  // Now let's write the records in a particular format:
  // 1 line per record
  // each field separated by a tab
  for a in A {
    // Depending on the situation, you might do:
    // writer.writeln(a.i, "\t", a.r, "\t", a.s);
    // that is equivalent to this (%t means any type):
    // writer.writef("%t\t%t\t%t\n", a.i, a.r, a.s);
    // but if you wanted to control precision/width and to
    // handle strings with tabs, you might use:
    writer.writef("%2i\t%2.2r\t%'S\n", a.i, a.r, a.s);
    // (%'S asks for a single-quoted string)

    // for debugging purposes, we also output it to stdout
    if debug then writef("%2i\t%2.2r\t%'S\n", a.i, a.r, a.s);
  }

  writer.close();
}

// Now read the data. Way 1: use formatted I/O
{
  var reader = f.reader();

  var rec:MyRecord;
  var i = 0;

  // clear B (only for our testing)
  if debug then B = rec;

  // read until we reach EOF
  // (note: if you want to handle format errors or I/O errors,
  //  you need to use error= versions of the I/O functions)
  while( reader.readf("%t\t%t\t%'S\n", rec.i, rec.r, rec.s) ) {
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

// Now read the data. Way 2: provide a readWriteThis method.
// Note that this didn't work with Chapel 1.11 or earlier.

/* notes on readWriteThis (see the language spec):
   - f is a Writer or a Reader
   - the compiler will generate readWriteThis for you if you don't
     provide one
   - the I/O operator <~> is available to read or write (depending
     on which situation we are being called in)
 */
proc MyRecord.readWriteThis(f) throws {
  f <~> i;
  f <~> new ioLiteral("\t");
  f <~> r;
  f <~> new ioLiteral("\t");

  // When doing the string I/O, we need to specify that we'd like
  // the string to be single-quoted. Unfortunately, readf is
  // not currently available on a Reader, so we have to rely
  // on the caller setting the string formatting with the channel's
  // style.
  // In the future, we hope to allow readf in this situation. 
  f <~> s;

  f <~> new ioLiteral("\n");
}

{
  // create a reader but specify that we'd like to use single-quoted strings.
  // 0x27 is ascii for '
  var reader = f.reader(style=new iostyle(string_format=iostringformat.basic:uint(8), string_start = 0x27, string_end = 0x27));

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


