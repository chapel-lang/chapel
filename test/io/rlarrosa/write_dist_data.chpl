/*
 * Block Distribution parallel IO Example
 *
 * This example demonstrates a simple use of the Block distribution to
 * distribute a 2D array of integers across multiple locales and then
 * doing a parallel write. Try it with 4 locales by running it with the
 * command-line argument -nl 4.  Note that you will need to set CHPL_COMM to
 * gasnet.  Then try it varying the numbers of locales and by changing the
 * problem size (--x_len=19 --y_len=13).
 *
 */

//
// Standard module BlockDist defines the Block distribution.
//
use BlockDist;

//
// Size of each dimension of our domain.
//
config const x_len = 19;
config const y_len = 13;
config param debugInfo = true;


var numloc:int(32)=numLocales;
config const filename:string="Arr_Bin_"+numloc+"_"+x_len+"_"+y_len+".dat";

//
// Declare and initialize an instance of the Block distribution Dist,
// a distributed domain Dom, and a distributed array A.  By default,
// the Block distribution distributes the domain across all locales.
//
var Dist = new dmap(new Block([1..x_len, 1..y_len]));
var Dom: domain(2) dmapped Dist = [1..x_len, 1..y_len];
var A: [Dom] int(64);

//
// Output the domain and array.  They print just like non-distributed
//   domains and arrays.
// Output of arrays are commented so if they are big not a lot of standard
//   output is done
//

writeln("Domain");
writeln(Dom);
writeln();

//
// Loop over the array using a parallel loop and assign each element
// an increasing number.
//
var j = 2;
forall i in Dom do {
  A(i) = (100+(i[2]-1)+(i[1]-1)*y_len);
}

// Print the array if it is not too big
if ( x_len < 20 ) && ( y_len < 20 ) then {
  writeln("Initialized array");
  writeln(A);
}


// Now we are going to write to disk

var fpos:int(64);
// Create an output file with the specified filename in write mode

var outfile = new file(filename, FileAccessMode.write);

// Open the file
outfile.open();

// Write the problem size in each dimension to the file

// write out the Distribution used
outfile.write(Dist);
fpos=outfile.chpl_ftell();

// The position of the file is written so they can be checked against the
// position when reading the file, when written it is written beginning with ww
// and when reading with rr, so an egrep "rr[1-9]|ww" file.out shows all the
// info needed to see if the positions are fine.

if debugInfo then writeln("ww1, ftell pos:",fpos," after writting Distribution");
// write out the Domain, althought only the distribution is used for now 
outfile.write(Dom);
// look were we are in the file, so we can see if it works fine, that is, if
// when we are going to read we are in the right place
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww2, ftell pos:",fpos," after writting Domain");

// write the values of the array to the file
outfile.write(A);
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww3, ftell pos:",fpos," after writting Array data");

outfile.close();


