use IO;

// Setup, ensuring the file is set up correctly to start and doesn't have old
// changes
var filename = "blah4.txt";
var f = open(filename, iomode.cw);
var wch = f.writer();
wch.write("Here is a file, I've counted it carefully.  It should be 77 characters long.\n");
wch.close();
f.close();

// Here's the actual meat of the test
var openreaderCheck1 = openreader(filename, start=5); // Should warn
var readRes: string;
openreaderCheck1.readLine(readRes, stripNewline=true);
writeln(readRes);
openreaderCheck1.close();

var openreaderCheck2 = openreader(filename, end=7); // Should warn
openreaderCheck2.readLine(readRes);
writeln(readRes);
openreaderCheck2.close();

var openreaderCheck3 = openreader(filename, start=5, end=7); // Should warn
openreaderCheck3.readLine(readRes);
writeln(readRes);
openreaderCheck3.close();

// Entering the file.reader phase
var f2 = open(filename, iomode.r);

var fileReaderCheck1 = f2.reader(start=5); // Should warn
fileReaderCheck1.readLine(readRes, stripNewline=true);
writeln(readRes);
fileReaderCheck1.close();

var fileReaderCheck2 = f2.reader(end=7); // Should warn
fileReaderCheck2.readLine(readRes);
writeln(readRes);
fileReaderCheck2.close();

var fileReaderCheck3 = f2.reader(start=5, end=7); // Should warn
fileReaderCheck3.readLine(readRes);
writeln(readRes);
fileReaderCheck3.close();

f2.close();

// Test file.lines
var f3 = open(filename, iomode.r);

for line in f3.lines(start=5) { // should warn
  writeln(line);
}

for line in f3.lines(end=7) { // should warn
  writeln(line);
}

for line in f3.lines(start=5, end=7) { // should warn
  writeln(line);
}

f3.close();

// Test file.writer

var f4 = open(filename, iomode.rw);

var fileWriterCheck1 = f4.writer(start=58); // should warn
fileWriterCheck1.writeln("6 characters long");
fileWriterCheck1.close();

var validateFileWriter1 = f4.reader();
validateFileWriter1.readLine(readRes, stripNewline=true);
writeln(readRes);
validateFileWriter1.close();

var fileWriterCheck2 = f4.writer(end=4); // should warn
fileWriterCheck2.write("Here");
fileWriterCheck2.close();

var validateFileWriter2 = f4.reader();
validateFileWriter2.readLine(readRes, stripNewline=true);
writeln(readRes);
validateFileWriter2.close();

var fileWriterCheck3 = f4.writer(start=29, end=31); // should warn
fileWriterCheck3.write("so");
fileWriterCheck3.close();

var validateFileWriter3 = f4.reader();
validateFileWriter3.readLine(readRes, stripNewline=true);
writeln(readRes);
validateFileWriter3.close();

f4.close();
