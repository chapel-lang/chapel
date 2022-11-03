use IO;

var filename = "seek.txt";
var readRes: string;
var readChEnd = openreader(filename, region=..5);
readChEnd.readLine(readRes);
writeln(readRes);
readChEnd.close();

var readChStartAndEnd = openreader(filename, region=3..5);
readChStartAndEnd.readLine(readRes);
writeln(readRes);
readChStartAndEnd.close();
