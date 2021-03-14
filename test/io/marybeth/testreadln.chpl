use IO;

config var inputfile = "smallHPL.dat";

var infile = open(inputfile, iomode.r).reader();
var outdevice: int;
var outfile : string ;

infile.readln();
infile.readln();
infile.readln(outfile);
infile.readln(outdevice);
writeln(outfile);
writeln(outdevice);

