use IO;

var filename = "seek.txt";
var f = open(filename, ioMode.r);
var readRes: string;

var readChEnd = f.reader(region=..5);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = f.reader(region=3..5);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
