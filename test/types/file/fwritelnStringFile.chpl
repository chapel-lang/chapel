use IO;

var myString = "You wear a nice shirt for a phone interview?";
var f = open("_test_fwritelnStringFile.txt", ioMode.cw).writer(locking=false);

f.writeln(myString);
f.close();

