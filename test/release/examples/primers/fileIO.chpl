/*
File I/O Primer
*/

// Some configuration to set up the examples.
config var n = 9,
           filename = "Arr.dat";
config const num = 128*1024;
config const example = 0;
config const testfile = "test.bin";
config const epsilon = 10e-13;

use IO;

/*
.. primers-io-textual-array

Textual Array I/O
-----------------
*/

// This is a simple example of using file I/O in Chapel. It initializes an array
// and writes its size and data to a file. It then opens the file, uses the size
// in the file to declare a new domain and array, and reads in the array data.
if example == 0 || example == 1 {

  // Create a domain of the specified problem size.
  const ADom = {1..n, 1..n};

  // Create and initialize an array of the specified size.
  var A: [ADom] real = [(i,j) in ADom] i + j/10.0;

  // Write the problem size and array out to the specified filename.
  writeSquareArray(n, A, filename);

  // Read an array in from the specified filename, storing in a new variable, B.
  var B = readArray(filename);

  // Print out B as a debugging step.
  writeln("B is:\n", B);

  // Verify that the values in A and B are within tolerance.
  const numErrors = + reduce [i in ADom] (abs(A(i) - B(i)) > epsilon);

  if (numErrors > 0) {
    writeln("FAILURE");
  } else {
    writeln("SUCCESS");
  }
}

// This procedure writes a square array out to a file.
proc writeSquareArray(n, X, filename) {

  // Create and open an output file with the specified filename in write mode.
  var outfile = open(filename, iomode.cw);
  var writer = outfile.writer();

  // Write the problem size in each dimension to the file.
  writer.writeln(n, " ", n);

  // Write out the array itself.
  writer.write(X);

  // Close the file.
  writer.close();
  outfile.close();
}

// This procedure reads a new array out of a file and returns it.
proc readArray(filename) {

   // Open an input file with the specified filename in read mode.
  var infile = open(filename, iomode.r);
  var reader = infile.reader();

  // Read the number of rows and columns in the array in from the file.
  var m = reader.read(int),
      n = reader.read(int);

  // Declare an array of the specified dimensions.
  var X: [1..m, 1..n] real;

  // Read in the array (row-major order is used for whole-array reads
  // like this).
  reader.read(X);

  // Close the file.
  reader.close();
  infile.close();

  // Return the array.
  return X;
}

/*
.. primers-io-binary

Binary I/O
----------
*/

//  Here we will write numbers ``0..n-1`` to a file in binary,
//  then we'll open it up and read the numbers in reverse,
//  to show how to seek in a file.

/*  There are two versions of this example:

      * a simple version
      * a slightly more complicated version that has some performance hints
*/

// Here comes the simple version!
if example == 0 || example == 2 {
  writeln("Running Example 2");

// First, open up a test file. Chapel's I/O interface allows
// us to open regular files, temporary files, memory, or file descriptors;
  var f = open(testfile, iomode.cwr);

// Since the typical 'file position' design leads to race conditions
// all over, the Chapel I/O design separates a file from a channel.
// A channel is a buffer to a particular spot in a file. Channels
// can have a start and and end, so that if you're doing parallel I/O
// to different parts of a file with different channels, you can
// partition the file to be assured that they do not interfere.

  {
    // Get a binary writing channel for the start of the file.
    var w = f.writer(kind=ionative);

    for i in 0..#num {
      var tmp:uint(64) = i:uint(64);
      w.write(tmp); // writing a uint(64) will write 8 bytes.
    }

    // Now w goes out of scope, flushing anything that is buffered.
    // Channels are reference-counted and should close when they go out
    // of scope, but we close it manually here to be sure.
    w.close();
  }

// Now that we have written our data file, we will read it backwards.
// Note: This could be a forall loop to do I/O in parallel!
  for i in 0..#num by -1 {
    var r = f.reader(kind=ionative, start=8*i, end=8*i+8);
    var tmp:uint(64);
    r.read(tmp);
    assert(tmp == i:uint(64));
    r.close();
  }

// Now close the file, even though they are reference-counted like channels.
// We can also remove the test file.
  f.close();
  unlink(testfile);
}

// Here is the slightly more complicated but faster version, using some hints.
// This time we're not going to use a temporary file, because
// we want to open it twice for performance reasons.
// If you want to measure the performance difference, try:

/* .. code-block:: sh

      time ./fielIOv2 --example=2
      time ./fielIOv2 --example=3
*/

if example == 0 || example == 3 {
  writeln("Running Example 3");

// First, open up a file and write to it.
  {
    var f = open(testfile, iomode.cwr);

    // When we create the writer, supplying locking=false will do unlocked I/O.
    // That's fine as long as the channel is not shared between tasks.
    var w = f.writer(kind=ionative, locking=false);

    for i in 0..#num {
      var tmp:uint(64) = i:uint(64);
      w.write(tmp);
    }

    w.close();
    f.close();
  }

// Now that we've created the file, when we open it for read access and hint
// 'random access' and 'keep data cached/assume data is cached',
// we can optimize better (using ``mmap``, if you like details).
  {
    var f = open(testfile, iomode.r,
                 hints=IOHINT_RANDOM|IOHINT_CACHED|IOHINT_PARALLEL);

    // This is a forall loop to do I/O in parallel!
    forall i in 0..#num by -1 {

      // When we create the reader, supplying locking=false will do unlocked I/O.
      // That's fine as long as the channel is not shared between tasks;
      // here it's just used as a local variable, so we are O.K.
      var r = f.reader(kind=ionative, locking=false, start=8*i, end=8*i+8);
      var tmp:uint(64);
      r.read(tmp);
      assert(tmp == i:uint(64));
      r.close();
    }

    f.close();
  }

  unlink(testfile);
}

/*
.. primers-io-utf8

Reading and printing UTF-8 lines
--------------------------------
*/

// Here we show that it's easy to read the lines in a file,
// including UTF-8 characters.
if example == 0 || example == 4 {
  writeln("Running Example 4");

  var f = open(testfile, iomode.cwr);
  var w = f.writer();

  w.writeln("Hello");
  w.writeln("This");
  w.writeln(" is ");
  w.writeln(" a test ");

// We only write the UTF-8 characters if unicode is supported,
// and that depends on the current unix locale environment
// (e.g. setting the environment variable ``LC_ALL=C`` will disable unicode support).
// Note that since UTF-8 strings are C strings, this should work even in a C locale.
// We don't do it all the time for testing sanity reasons.
  if unicodeSupported() then w.writeln(" of UTF-8 Euro Sign: €");

  // flush buffers, close the channel.
  w.close();

  var r = f.reader();
  var line:string;
  while( r.readline(line) ) {
    write("Read line: ", line);
  }
  r.close();

  // Or, if we just want all the lines in the file, we can use file.lines,
  // and we don't even have to make a reader:
  for line in f.lines() {
    write("Read line: ", line);
  }

  f.close();
  unlink(testfile);
}

/*
.. primers-io-error-handling

Error handling
--------------
*/

// Now we will show that error handling is possible
// with the new I/O routines. Maybe one day this strategy will
// be replaced by something more durable, but until then,
// Chapel programs can still respond to errors.
if example == 0 || example == 5 {
  writeln("Running Example 5");

  try! {
    // Who knows, maybe 1st unlink succeeds.
    unlink(testfile);

    // File does not exist by now, for sure.
    unlink(testfile);

    assert(false); // never reached
  } catch e: SystemError {
    assert(e.err == ENOENT);
  }

  try! {
    // What happens if we try to open a non-existent file?
    var f = open(testfile, iomode.r);

    assert(false); // never reached
  } catch e: SystemError {
    assert(e.err == ENOENT);
  }
}

// Note that if an ``error=`` argument is not supplied to an
// I/O function, it will call ``ioerror``, which will
// in turn halt with an error message.

/*
.. primers-io-object-writing

Object-at-a-time writing
------------------------
*/

// In this example that output from multiple tasks can use the same channel,
// and each ``write()`` call will be completed entirely before another one
// is allowed to begin.
if example == 0 || example == 6 {
  writeln("Running Example 6");

  forall i in 1..4 {
    writeln("This should be a chunk: {", "\n a", "\n b", "\n}");
  }

  record MyThing {
    proc writeThis(w) throws {
      w.writeln("This should be a chunk: {");
      w.writeln(" a");
      w.writeln(" b");
      w.writeln("}");
    }
  }

  forall i in 1..4 {
    var t:MyThing;
    write(t);
  }
}

/*
.. primers-io-bits

Binary I/O with bits at a time
------------------------------
*/

// Here we demonstrate bit-level I/O.
if example == 0 || example == 7 {
  writeln("Running Example 7");

  var f = open(testfile, iomode.cwr);

  {
    var w = f.writer(kind=ionative);

    // Write 011 0110 011110000
    w.writebits(0b011, 3);
    w.writebits(0b0110, 4);
    w.writebits(0b011110000, 9);
    w.close();
  }

  // Try reading it back the way we wrote it.
  {
    var r = f.reader(kind=ionative);
    var tmp:uint(64);

    r.readbits(tmp, 3);
    assert(tmp == 0b011);

    r.readbits(tmp, 4);
    assert(tmp == 0b0110);

    r.readbits(tmp, 9);
    assert(tmp == 0b011110000);

    r.close();
  }

  // Try reading it back all as one big chunk.
  // Read 01101100 11110000
  {
    var r = f.reader(kind=ionative);
    var tmp:uint(8);

    r.read(tmp);
    assert(tmp == 0b01101100);

    r.read(tmp);
    assert(tmp == 0b11110000);

    r.close();
  }

  f.close();
}
