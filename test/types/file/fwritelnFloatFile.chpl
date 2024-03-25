use IO;

var myFloat = 9.99;
var f = open("_test_fwritelnFloatFile.txt", ioMode.cw).writer(locking=false);

f.writeln(myFloat);
f.close();

