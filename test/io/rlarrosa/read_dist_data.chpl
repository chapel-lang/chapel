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

// We are going to read the file

var infile = new file(filename, FileAccessMode.read);
infile.open();

// Read the distribution used
var bb=read2DBlock(infile);

// Create the original distribution
var Distrib = new dmap(new Block(rank=2, bb));                                          

var fpos=infile.chpl_ftell();
if debugInfo then writeln("rr1, ftell pos:",fpos," after reading the distribution");

// Create the original domain
bb=read2DBlock(infile);
var Dom: domain(2) dmapped Distrib = bb;

// The read doesn't change the already defined values
//infile.read(Dom2);
writeln("Read domain is:");
writeln(Dom);

// Creates a new Array to read from the file
var Aread: [Dom] int(64);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr2, ftell pos:",fpos," after reading the domain");
infile.read(Aread);
fpos=infile.chpl_ftell();
if debugInfo then writeln("rr3, ftell pos:",fpos," after reading the array data");
if ( x_len < 20 ) && ( y_len < 20 ) then {
	writeln(" The loaded array is:");
	writeln(Aread);
}
infile.close();

