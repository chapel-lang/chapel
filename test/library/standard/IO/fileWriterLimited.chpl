use IO;

var filename = "fileWriterLimited.txt";
var f = open(filename, iomode.cw);

var writeCh = f.writer();
writeCh.writeln("She said she was looking for me");
writeCh.close();

var writeChStart = f.writer(region=29..);
writeChStart.write("us");
writeChStart.close();

var writeChEnd = f.writer(region=..2);
writeChEnd.write("You");
writeChEnd.close();

var writeChStartAndEnd = f.writer(region=9..11);
writeChStartAndEnd.write("Ned");
writeChStartAndEnd.close();

f.close();
