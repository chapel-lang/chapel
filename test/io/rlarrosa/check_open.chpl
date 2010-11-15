
config const filename:string="./A_borrar.bor";

//var res=chpl_remove(filename);
var res:int(32);

// Create an output file with the specified filename in write mode
var outfile = new file(filename, FileAccessMode.write);

outfile.open();

outfile.writeln(5," ",6," ",7," ",8," ",9.4);
outfile.writeln(10.6);

// Read the current pos inside the file
var fpos=outfile.chpl_ftell();
if (fpos!=17) then writeln("Error with file position ", fpos);

outfile.close();


var infile = new file(filename, FileAccessMode.read);

infile.open();
var a,b,c:real;
infile.read(a,b,c);
writeln(a," ",b," ",c);
infile.read(a,b,c);
writeln(a," ",b," ",c);
fpos=infile.fseekset(4);
infile.read(a,b,c);
fpos=infile.chpl_ftell();
writeln("Using fseekset 6:",a," ",b," ",c, " fpos:",fpos);
fpos=infile.fseekrel(-10);
infile.read(a,b,c);
fpos=infile.chpl_ftell();
writeln("Using fseek -10:",a," ",b," ",c, " fpos:",fpos);
infile.close();


outfile = new file(filename, FileAccessMode.append);
outfile.open();
outfile.writeln("appended to file");
outfile.close();

infile = new file(filename, FileAccessMode.read);
infile.open();
infile.fseekset(fpos);
var str:string;
infile.read(str);
writeln("string read:",str);
infile.read(str);
writeln("string read:",str);
infile.read(str);
writeln("string read:",str);
infile.close();

/*


res=chpl_remove(filename);
if (res!=0) then writeln("Error deleting file ", filename);

res=chpl_remove(filename);
if (res!=0) then writeln("Correct, file cannot be deleted twice");


*/
