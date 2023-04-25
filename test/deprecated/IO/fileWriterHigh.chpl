use IO;

var filename = "fileWriterHigh.txt";
var f = open(filename, ioMode.cw);

var writeCh = f.writer();
writeCh.writeln("She said she was looking for me");
writeCh.close();
var writeChEnd = f.writer(region=..3);
writeChEnd.write("You");
writeChEnd.close();

var writeChStartAndEnd = f.writer(region=9..12);
writeChStartAndEnd.write("Ned");
writeChStartAndEnd.close();

f.close();
