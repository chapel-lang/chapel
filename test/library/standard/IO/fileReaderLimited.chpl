use IO;

var filename = "fileReaderLimited.txt";
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

var readChEnd = f.reader(region=..5);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = f.reader(region=3..5);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
