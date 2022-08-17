use IO;

var filename = "openwriterEndBug.txt";
var writeCh = openwriter(filename);
writeCh.writeln("She said she was looking for me");
writeCh.close();

var writeChEnd = openwriter(filename, end=3);
writeChEnd.write("You");
writeChEnd.close();
