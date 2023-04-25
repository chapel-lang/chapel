use IO;

var myComplex: complex = 1.2 + 2.3i;

var f = open("_test_fwritelnComplexFile.txt", ioMode.cw).writer();

f.writeln(myComplex);
f.close();
