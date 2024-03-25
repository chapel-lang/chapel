use IO;

var filename = "openreaderLimited.txt";
var f = open(filename, ioMode.r);
var readCh = f.reader(locking=false);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();

var readChStart = f.reader(region=3.., locking=false);
readChStart.readLine(readRes, stripNewline=true);
writeln(readRes);
readChStart.close();

var readChEnd = f.reader(region=..4, locking=false);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = f.reader(region=3..4, locking=false);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
