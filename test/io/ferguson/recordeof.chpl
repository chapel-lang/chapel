/* This example demonstrates how to use formatted I/O to
   write and read a record stored as a tab-separated
   fields, with one record per line.
 */

/* MyRecord contains fields of various types in order to demonstrate
   functionality.
 */
record MyRecord {
  var i: int;
//  var r: real;
//  var s: string;
}

config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, iomode.cwr);

// Now read the data. Way 2: provide a readWriteThis method.

/* notes on readWriteThis (see the language spec):
   - f is a Writer or a Reader
   - the compiler will generate readWriteThis for you if you don't
     provide one
   - the I/O operator <~> is available to read or write (depending
     on which situation we are being called in)
 */
proc MyRecord.readWriteThis(f) {
  f <~> i;
//  f <~> new ioLiteral("\t");
//  f <~> r;
//  f <~> new ioLiteral("\t");

  // When doing the string I/O, we need to specify that we'd like
  // the string to be single-quoted. Unfortunately, readf is
  // not currently available on a Reader, so we have to rely
  // on the caller setting the string formatting with the channel's
  // style.
  // In the future, we hope to allow readf in this situation. 
//  f <~> s;

  f <~> new ioLiteral("\n");
}

{
  // create a reader but specify that we'd like to use single-quoted strings.
  // 0x27 is ascii for '
  var reader = f.reader(style=new iostyle(string_format=QIO_STRING_FORMAT_BASIC, string_start = 0x27, string_end = 0x27));

  var rec:MyRecord;
  var i = 1;

  // read until we reach EOF
  // (note: if you want to handle format errors or I/O errors,
  //  you need to use error= versions of the I/O functions)
  while( reader.read(rec) ) {
    writeln("read ", rec);
  }

  writeln("Done");

  reader.close();
}


