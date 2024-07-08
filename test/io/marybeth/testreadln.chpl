use IO;

config var inputfile = "smallHPL.dat";

var infile = open(inputfile, ioMode.r).reader(locking=false);
var outdevice: int;
var outfile : string ;

infile.readln();
infile.readln();
infile.readln(outfile);
infile.readln(outdevice);
writeln(outfile);
writeln(outdevice);

