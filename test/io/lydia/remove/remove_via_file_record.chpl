// Really stupid little test.
// Opens a file, writes to it, reads that result, prints it, deletes
// the file.
var filename = "remove_via_file_record.txt";
var f = open(filename, iomode.cwr);
var writeSomething = f.writer();
writeSomething.writeln("Something");
writeSomething.close();
var result: string;
var readItBack = f.reader();
readItBack.read(result);
readItBack.close();
writeln(result);
f.remove();
