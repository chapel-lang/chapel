use IO;

var filename = "openreaderLimited.txt";
var f = open(filename, iomode.r);
var readCh = f.reader();
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();

var readChStart = f.reader(region=3..);
readChStart.readLine(readRes, stripNewline=true);
writeln(readRes);
readChStart.close();

var readChEnd = f.reader(region=..4);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = f.reader(region=3..4);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
