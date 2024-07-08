use IO;

var filename = "openreaderLimited.txt";
var readCh = openReader(filename, locking=false);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();

var readChStart = openReader(filename, region=3.., locking=false);
readChStart.readLine(readRes, stripNewline=true);
writeln(readRes);
readChStart.close();

var readChEnd = openReader(filename, region=..4, locking=false);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = openReader(filename, region=3..4, locking=false);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
