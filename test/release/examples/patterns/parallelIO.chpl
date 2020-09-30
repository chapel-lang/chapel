// This program is designed to demonstrate the use of multiple files
// and/or channels to do parallel binary I/O in Chapel.  By design, it
// is something of a demonstration vehicle for I/O concepts, and not
// necessarily a realistic use-case.
//
// First, we'll write a 2D block-distributed array with a 1D
// decomposition to a file by having each locale seek to its
// appropriate offset and write out its local chunk of data.
//
// Then, we'll read the data back into a completely local array using
// multiple tasks (note: likely using a different number of tasks than
// the number of locales).
//
// Variations on these patterns could be used to...
// - use multiple locales _and_ tasks per locale to do the writes/reads
// - use different data decompositions for the arrays
// - use a text file format rather than binary
// - use multiple files rather than a single one
// - etc.

// load some standard modules in order to...
//
use BlockDist,   // get the Block distribution for arrays
    IO,          // get access to the file and channel types
    RangeChunk;  // break a range into chunks


// declare some configuration constants for:
//
config const n = 100,                  // the array size per dimension
             filename = "array.dat",   // the filename to use
             numTasks = here.numPUs(), // the number of tasks for reading
             printArray = true,        // whether to print the final array
             debug = false;            // whether to print debugging output
       

// Create a copy of the locales array that arranges them in a
// "vertical" arrangement so that each one gets a contiguous block
// slab of elements (Chapel arrays are laid out in row-major order by
// default).  This permits us to do one seek per locale (rather than a
// seek per row) when using a block distribution, for simplicity in
// this exercise.
//
const targetLocales = reshape(Locales, {1..numLocales, 1..1});


// Create a distributed domain and array targeting our view of the
// locales.
//
const D = {0..<n, 0..<n} dmapped Block({0..<n, 0..<n}, targetLocales);
var A: [D] real;


// Initialize the array with some distinctive values
//
forall (i,j) in D do
  A[i,j] = i + j / n:real;


// Create a new file for writing, and write the array's size:
//
var outfile = open(filename, mode=iomode.cw,
                   style=new iostyle(binary=true));
var outCh = outfile.writer();
outCh.write(n);
outCh.close();
outfile.close();


// Create a task per locale, on that locale, and have each one seek to
// its appropriate spot in the file and write its local chunk of the
// array.
//
coforall loc in targetLocales {
  on loc {
    // Which indices of A does my locale own?
    //
    const myIndices = D.localSubdomain();
    const (myStartRow, myStartCol) = myIndices.low;
    assert(myStartCol == 0);


    // Where should I start writing in the file?
    //
    const skip = numBytes(int)            // the array size we wrote previously
               + myStartRow * n * numBytes(real);     // the rows prior to ours


    // Sanity check, if requested
    //
    if debug then
      writeln("Locale ", here.id, " owns ", myIndices, " so skips ", skip);

    // Get a reference to this locale's local block of data
    //
    ref myA = A.localSlice[myIndices];

    // open the file again and an unlocked writing channel to it.
    // Note that accessing a file on a remote locale currently doesn't
    // work (this is an area where we don't currently paper over the
    // fact that different locales are distinct processes).  This is
    // why we're opening the file again here rather than simply
    // referring to 'outfile' from above.
    // 
    var outCh = open(filename, mode=iomode.rw,
                     style=new iostyle(binary=true)).writer(locking=false);
    

    // seek to the right spot in the file and write our local elements.
    //
    outCh.seek(skip);
    outCh.write(myA);
  } // the channel and file will automatically be closed when they leave scope
}


// OK, time to read the data back in now!

// Declare a new array; just to switch things up, let's make it a
// completely local array and read into it with multiple tasks.
//
var B: [0..<n, 0..<n] real;


// Open the file for reading and make sure the problem size matches
//
var input = open(filename, mode=iomode.r,
                 style=new iostyle(binary=true));
var inCh = input.reader();
var nCheck: int = inCh.read(int);
assert(n == nCheck, "size mismatch: " + n:string + " != " + nCheck:string);


// Use the 'RangeChunk' library to chop rows '0..<n' into 'numTasks'
// tasks, and create a parallel task for each sub-range that results
// using a coforall.
//
coforall myRows in chunks(0..<n, numTasks) {
  if debug then
    writeln("I own rows: ", myRows);

  // Open my own channel to the file.  Since we're still on the same
  // locale, we can refer to 'input' directly.
  //
  var myInCh = input.reader(locking=false);

  // Compute my task's offset, skip to that position in the file, and
  // then read into the appropriate slice of 'B'
  //
  const skip = numBytes(int)        // the array size we wrote previously
             + myRows.low * n * numBytes(real);     // rows prior to ours
  myInCh.seek(skip);
  myInCh.read(B[myRows, ..]);
}


// Check our work: Make sure we read the same thing we wrote (note that
// this is comparing elements of a distributed array to a local one)
//
if && reduce (A == B) then
  writeln("I/O Successful!");
else
  forall (i,j) in D do
    if A[i,j] != B[i,j] then
      writeln("Mismatch at element ", (i,j), ": ", A[i,j], " != ", B[i,j]);


// Print the array, if requested
//
if printArray then
  writeln("B is:\n", B);
