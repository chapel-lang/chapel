use IO;

var myFloat = 9.99;
var f = open("_test_fwritelnFloatFile.txt", iomode.cw).writer();

f.writeln(myFloat);
f.close();

