use IO;

var filename = "openreaderLimited.txt";
var readCh = openReader(filename, region=-5.., locking=false);
var readRes: string;
readCh.readLine(readRes, stripNewline=true);
writeln(readRes);
readCh.close();
