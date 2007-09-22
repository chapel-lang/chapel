config var inputfile = "smallHPL.dat";

var infile = file(inputfile, path='./', mode='r');
var outdevice: int;
var outfile : string ;

infile.open();
infile.readln();
infile.readln();
infile.readln(outfile);
infile.readln(outdevice);
writeln(outfile);
writeln(outdevice);

