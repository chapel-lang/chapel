use IO;

var filename = "openreaderLimited.txt";
var readCh = openreader(filename);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();

var readChStart = openreader(filename, region=3..);
readChStart.readLine(readRes, stripNewline=true);
writeln(readRes);
readChStart.close();