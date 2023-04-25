use IO;

var filename = "openreaderLimited.txt";
var readCh = openReader(filename);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();

var readChStart = openReader(filename, region=3..);
readChStart.readLine(readRes, stripNewline=true);
writeln(readRes);
readChStart.close();

var readChEnd = openReader(filename, region=..4);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = openReader(filename, region=3..4);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
