/*
Copied from the sample apps
*/
use IO;

config var n = 9,                 // the problem size
           filename = "Arr.dat";  // the filename for writing/reading the array


proc main {

  const ADom = {1..n, 1..n};  // Create a domain of the specified problem size

  // Create and initialize an array of the specified size
  var A: [ADom] real = [(i,j) in ADom] i + j/10.0;
  
  for i in 1..10
  {
        writeSquareArray(n,A*i,"Arr_" + i:string +".txt");
  }
  
  var (Avg,Std) = AverageImage("Arr",10);

  writeln("Avg is:\n", Avg);
  writeln("Std is:\n", Std);
  
}


//
// this function writes a square array out to a file
//
proc writeSquareArray(n, X, filename) {
  // Create an output file with the specified filename in write (iomode.cw) mode
  var outfile = open(filename, iomode.cw);
  var writer = outfile.writer();

  // Write the problem size in each dimension to the file
  writer.writeln(n, " ", n);

  // write out the array itself
  writer.write(X);

  // close the file
  writer.close();
  outfile.close();
}


//
// This function reads a new array out of a file and returns it
//
proc readArray(filename) {
  // Create an input file with the specified filename in read (iomode.r) mode
  var infile = open(filename, iomode.r);
  var reader = infile.reader();

  // Read the number of rows and columns in the array in from the file
  var m = reader.read(int), 
      n = reader.read(int);

  // Declare an array of the specified dimensions
  var X: [1..m, 1..n] real;

  //
  // Read in the array elements one by one (eventually, you should be
  // able to read in the array wholesale, but this isn't currently
  // supported.
  //
  for i in 1..m do
    for j in 1..n do
      reader.read(X(i,j));

  // Close the file
  reader.close();
  infile.close();

  // Return the array
  return X;
}

proc SQR(const A)
{
	var R = A;
	forall (i) in (A.domain) do R(i) *= R(i);
	return R;
}	

proc AverageImage(filename, n)
{

        var image = readArray( filename + "_" +1:string +".txt" );  //image values ideal range = 0-1, single channel
	var sum = image;
	var sumSq = SQR(image);
	
	for i in 2..n do
	{
                var image = readArray(filename + "_" +i:string +".txt");
		sum += image;
		//writeln("image is:\n", image);
		//writeln("Sum is:\n", sum);
		sumSq += SQR(image);
	}	
	
	sum /= n;
	var S = (sumSq / n) -  SQR(sum);  //mean of squares - square of mean - should be ok overflow is low
    
	return ( sum, S );
}
