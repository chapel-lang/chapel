use IO;

var filename = "openwriterStartBug.txt";
var writeCh = openwriter(filename);
writeCh.writeln("She said she was looking for me");
writeCh.close();

var writeChStart = openwriter(filename, start=31);
writeChStart.write("us");
writeChStart.close();
