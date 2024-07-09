use IO;

var filename = "fileWriterLimited.txt";
var f = open(filename, ioMode.cw);

var writeCh = f.writer(locking=false);
writeCh.writeln("She said she was looking for me");
writeCh.close();

var writeChStart = f.writer(region=29.., locking=false);
writeChStart.write("us");
writeChStart.close();

var writeChEnd = f.writer(region=..2, locking=false);
writeChEnd.write("You");
writeChEnd.close();

var writeChStartAndEnd = f.writer(region=9..11, locking=false);
writeChStartAndEnd.write("Ned");
writeChStartAndEnd.close();

f.close();
