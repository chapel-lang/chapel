use IO;

var A: [1..2] string;
var B: [1..2, 1..2] string;

var infile = open("readDifferentRankArrays.dat", iomode.r);
var f = infile.reader();

f.readf("%ht", A);
writeln(A);
try {
  f.readf("%ht", B);
} catch e {
  writeln("Caught: ", e);
}

readf("%ht", B);

