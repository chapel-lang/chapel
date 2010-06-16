/*
 * Block Distribution parallel IO Example
 *
 * This example demonstrates a simple use of the Block distribution to
 * distribute a 2D array of integers across multiple locales and then
 * doing a parallel write and finally a read from disk. Try it
 * with 4 locales by running it with the command-line argument -nl 4.
 * Note that you will need to set CHPL_COMM to gasnet.  Then try it
 * varying the numbers of locales and by changing the problem size
 * (--n=16).
 *
 */

//
// Standard module BlockDist defines the Block distribution.
//
use BlockDist;

//
// Size of each dimension of our domain.
//
config const n = 8;

var numloc:int(32)=numLocales;
config const filename:string="Arr_Bin_"+numloc+"_"+n+".dat";

//
// Declare and initialize an instance of the Block distribution Dist,
// a distributed domain Dom, and a distributed array A.  By default,
// the Block distribution distributes the domain across all locales.
//
var Dist = new dmap(new Block([1..n, 1..n]));
var Dom: domain(2) dmapped Dist = [1..n, 1..n];
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

//writeln("Array");
//writeln(A);
//writeln();

//
// Output the locale ID numbers for each array element, determining
// this information by applying ".locale" to each array element.
//
/*
writeln("Distribution Map");
for i in 1..n {
  for j in 1..n {
    write(A(i,j).locale.id, " ");
  }
  writeln();
}
writeln();
*/

//
// Loop over the array using a parallel loop and assign each element
// and increasing number.
//
var j = 2;
forall i in Dom do {
  A(i) = (50+i[2]+(i[1]-1)*n)+1;
}
//writeln("Initialized array");
//writeln(A);

var fpos:int(64);
  // Create an output file with the specified filename in write mode

var outfile = new file(filename, FileAccessMode.write);
outfile.open();
outfile.close();

outfile = new file(filename, FileAccessMode.readwrite);

// Open the file
outfile.open();

// Write the problem size in each dimension to the file
//  outfile.writeln(n, " ", n);

// write out the array itself
outfile.write(Dist);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww1, ftell:",fpos);
outfile.write(Dom);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww2, ftell:",fpos," A");
outfile.write(A);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww3, ftell:",fpos," Dom");
outfile.write(Dom);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww4, ftell:",fpos);
outfile.write(Dist);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww5, ftell:",fpos);
outfile.write(A);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("ww6, ftell:",fpos);

// close the file
outfile.close();

writeln("Zeroed via parallel iteration over the array");

forall a in A do {
  a = 0;
}

// writeln(A);
//  var Distrib: Dist = [1..5, 1..5];
var Distrib = new dmap(new Block(rank=2, boundingBox=[1..5, 1..5]));                                          
var Dom2: domain(2) dmapped Distrib = [1..5,1..5];

outfile.open();
outfile.read(Distrib);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr1, ftell:",fpos);

// writeln(Distrib);
outfile.read(Dom2);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr2, ftell:",fpos," A");
// writeln(Dom2);
outfile.read(A);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr3, ftell:",fpos," Dom");
// writeln(A);
outfile.read(Dom2);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr4, ftell:",fpos," Dis");
outfile.read(Distrib);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr5, ftell:",fpos," A");

forall a in A do {
  a = 0;
}
writeln(Dom2);
outfile.read(A);
fpos=outfile.chpl_ftell();
if debugBlockDist then writeln("rr6, ftell:",fpos);
// writeln(A);
outfile.close();

//
// In parallel, subtract one from each element of the array.
//
forall i in Dom do {
  A(i) = A(i) - 1;
}
//writeln("Subtracted 1 via parallel iteration over the domain");
//writeln(A);
//writeln();

//
// In parallel, iterate over the array and set each element to 0.
//
forall a in A do {
  a = 0;
}
writeln("Zeroed via parallel iteration over the array");
//writeln(A);

