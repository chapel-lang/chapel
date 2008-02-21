config var inputfile = "smallHPL.dat";

var infile = new file(inputfile, path='./', mode=FileAccessMode.read);
var outdevice: int;
var outfile : string ;

infile.open();
infile.readln();
infile.readln();
infile.readln(outfile);
infile.readln(outdevice);
writeln(outfile);
writeln(outdevice);

