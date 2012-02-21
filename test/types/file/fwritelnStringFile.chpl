var myString = "You wear a nice shirt for a phone interview?";
var f = open("_test_fwritelnStringFile.txt", iomode.w).writer();

f.writeln(myString);
f.close();

