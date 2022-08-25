use IO;

var filename = "openreaderLimited.txt";
var f = open(filename, iomode.r);
var readCh = f.reader(region=-1..0);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();
f.close();
