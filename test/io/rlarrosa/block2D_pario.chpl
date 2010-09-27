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

if ( x_len<10 ) then halt("X size too small, must be at least 10");
if ( y_len<6 ) then halt("Y size too small, must be at least 6");

var Dom: domain(2) dmapped Dist = [5..(x_len-5), 3..(y_len-3)];
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
for i in 1..x_len {
  for j in 1..y_len {
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
  A(i) = (50+i[2]+(i[1]-1)*y_len)+1;
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
if debugInfo then writeln("ww1, ftell:",fpos);
// write out the Domain, althought only the distribution is used for now 
outfile.write(Dom);
// look were we are in the file, so we can see if it works fine
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww2, ftell:",fpos," A");
// write the values of the array
outfile.write(A);
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww3, ftell:",fpos," Dom");
// write the domain again, to make the file more complicated

Dom = [5..(x_len-3), 3..(y_len-2)];

outfile.write(Dom);
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww4, ftell:",fpos);
// write the array again, to make the file more complicated
outfile.write(A);
fpos=outfile.chpl_ftell();
if debugInfo then writeln("ww5, ftell:",fpos);
// close the file
outfile.close();

writeln("Zeroed via parallel iteration over the array");
forall a in A do {
  a = 0;
}


// Now we are going to read what we have written

var infile = new file(filename, FileAccessMode.read);
infile.open();

// Read the distribution used
var bb=readDist(infile);

// This must be redone so if the distribution and domains aren't the same it works.
// I will work on that ASAP.

// Create the original distribution
var Distrib = new dmap(new Block(rank=2, bb));                                          
//writeln("Distribution read:");
//writeln(Distrib);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr1, ftell:",fpos);
// Create the original domain
bb=readDist(infile);
var Dom2: domain(2) dmapped Distrib = bb;


// The read doesn't change the already defined values
//infile.read(Dom2);
writeln("First domain:");
writeln(Dom2);

// Creates a new Array to read from the file
var Aread: [Dom2] int(64);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr2, ftell:",fpos," A");
infile.read(Aread);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr3, ftell:",fpos," Dom");
if ( x_len < 20 ) && ( y_len < 20 ) then {
	writeln(" The loaded array is:");
	writeln(Aread);
}
// Read the second domain
bb=readDist(infile);
Dom2 = bb;
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr4, ftell:",fpos," Dis");
forall a in Aread do {
  a = 0;
}
writeln("Second domain:");
writeln(Dom2);
infile.read(Aread);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr5, ftell:",fpos);
if ( x_len < 20 ) && ( y_len < 20 ) then {
	writeln(" The loaded array is:");
	writeln(Aread);
}
infile.close();

//
// In parallel, subtract one from each element of the array.
//
forall i in Dom do {
  Aread(i) = Aread(i) - 1;
}
//writeln("Subtracted 1 via parallel iteration over the domain");
//writeln(A);
//writeln();

//
// In parallel, iterate over the array and set each element to 0.
//
forall a in Aread do {
  a = 0;
}
writeln("Zeroed via parallel iteration over the array");
//writeln(A);



// Function to read a distribution from a file
// 	it will be moved to some common file so it can be called 
// 	by any program, but I must review its interface and its working
// 	with the domains a little more.

def readDist(f : file) {
  var gres=0:int(64);
  var gerr=0:int;
  
  var fpos=f.chpl_ftell();
  writeln("readBinBlock begin, ftell:",fpos);

  var rank:int;
  binfread(rank,4,1,f._fp,gres,gerr);
  writeln("Read Block rank:",rank);

  var rr:int;
  var bb:domain(2);
  var low: [1..2] int;
  var high: [1..2] int;
  var stride: [1..2] int;

  for dim in 1..rank do {
    binfread(low[dim],4,1,f._fp,gres,gerr);
    binfread(high[dim],4,1,f._fp,gres,gerr);
    binfread(stride[dim],4,1,f._fp,gres,gerr);
                writeln("Read Block dim:",dim," : ",low[dim]);
                writeln("Read Block dim:",dim," : ",high[dim]);
                writeln("Read Block dim:",dim," : ",stride[dim]);
  }
  bb=[low[1]..high[1] by stride[1],low[2]..high[2] by stride[2]];
  writeln("readBlock bb:",bb);
  fpos=f.chpl_ftell();
  writeln("readBinBlock end, ftell:",fpos);
  return bb;
}
