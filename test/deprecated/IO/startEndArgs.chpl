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
var val = openreaderCheck1.readLine(readRes, stripNewline=true);
writeln(readRes);
openreaderCheck1.close();
