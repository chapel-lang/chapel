use IO;

var filename = "blah5.txt";
var ch1 = openwriter(filename, start=0); // Should warn
ch1.write("Here is a file, I've counted it carefully.  It should be 77 characters long.\n");
ch1.close();

var check1 = openreader(filename);
var readRes: string;
check1.readLine(readRes, stripNewline=true);
writeln(readRes);
check1.close();

var ch2 = openwriter(filename, end=78); // Should warn
ch2.write("This is a file, I've counted it carefully.  It should be 77 characters long.\n");
ch2.close();

var check2 = openreader(filename);
check2.readLine(readRes, stripNewline=true);
writeln(readRes);
check2.close();

var ch3 = openwriter(filename, start=0, end=78); // Should warn
ch3.write("Here is a file, I've counted it carefully.  It should be 77 characters long.\n");
ch3.close();

var check3 = openreader(filename);
check3.readLine(readRes, stripNewline=true);
writeln(readRes);
check3.close();
