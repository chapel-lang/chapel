/*
 * This test shows a simple example of using file I/O in Chapel.  It
 * initializes an array and writes its size and data to a file.  It
 * then opens the file, uses the size in the file to declare a new
 * domain and array, and reads in the array data.
 */

config var n = 9,                 // the problem size
           filename = "Arr.dat";  // the filename for writing/reading the array


def main {
  const ADom = [1..n, 1..n];  // Create a domain of the specified problem size

  // Create and initialize an array of the specified size
  var A: [(i,j) in ADom] real = i + j/10.0;
  
  // Write the problem size and array out to the specified filename
  writeSquareArray(n, A, filename);
  
  // Read an array in from the specified filename, storing in a new variable, B
  var B = readArray(filename);
  
  // Print out B as a debugging step
  writeln("B is:\n", B);
  
  //
  // verify that A and B contain the same values and print success or failure
  //
  const numErrors = + reduce [i in ADom] (A(i) != B(i));

  if (numErrors > 0) {
    writeln("FAILURE");
  } else {
    writeln("SUCCESS");
  }
}


//
// this function writes a square array out to a file
//
def writeSquareArray(n, X, filename) {
  // Create an output file with the specified filename in write ("w") mode
  var outfile = new file(filename, "w");

  // Open the file
  outfile.open();

  // Write the problem size in each dimension to the file
  outfile.writeln(n, " ", n);

  // write out the array itself
  outfile.write(X);

  // close the file
  outfile.close();
}


//
// This function reads a new array out of a file and returns it
//
def readArray(filename) {
  // Create an input file with the specified filename in read ("r") mode
  var infile = new file(filename, "r");

  // Open the file
  infile.open();

  // Read the number of rows and columns in the array in from the file
  var m = infile.read(int), 
      n = infile.read(int);

  // Declare an array of the specified dimensions
  var X: [1..m, 1..n] real;

  //
  // Read in the array elements one by one (eventually, you should be
  // able to read in the array wholesale, but this isn't currently
  // supported.
  //
  for i in 1..m do
    for j in 1..n do
      infile.read(X(i,j));

  // Close the file
  infile.close();

  // Return the array
  return X;
}


