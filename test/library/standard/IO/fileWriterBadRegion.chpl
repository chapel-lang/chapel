use IO;

var filename = "fileWriterBadRegion.txt";
var f = open(filename, ioMode.cw);
var writeCh = f.writer(region=-1..);
writeCh.writeln("blah blah blah");
writeCh.close();
f.close();
