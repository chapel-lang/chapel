module dissimilarity {

  use IO;

  // This procedure reads a new array out of a file and returns it.
  proc ReadArray(filename : string) throws {

    // Open an input file with the specified filename in read mode.
    var infile = open(filename, ioMode.r);
    var reader = infile.reader();

    // Read the number of rows and columns in the array in from the file.
    var m = reader.read(int);

    // Declare an array of the specified dimensions.
    var X: [1..m, 1..m] real(32);

    // Read in the array (row-major order is used for whole-array reads
    // like this).
    reader.read(X);

    /*
    // In the old beta diversity formula the dissimilarity
    // only enters as a squared quantity * 10000
    for i in X.domain do {
      X[i] = 10000 * X[i]**2;
    }
    */

    // Close the file.
    reader.close();
    infile.close();

    // Return the array.
    return (X, m);
  }

}
