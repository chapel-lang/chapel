
var numloc:int(32)=numLocales;
config const filename:string="A_borrar.bor";

var fpos:int(64)=6000000000;
// Create an output file with the specified filename in write mode

chpl_remove(filename);

var outfile = new file(filename, FileAccessMode.write);

// Open the file
outfile.open();

fpos=outfile.fseekset(fpos);
outfile.writeln("hello world");
fpos=outfile.chpl_ftell();
writeln("actual fpos:",fpos);
outfile.close();

//chpl_remove(filename);

