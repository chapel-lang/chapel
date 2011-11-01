var myComplex: complex = 1.2 + 2.3i;

var f = open("_test_fwritelnComplexFile.txt", mode.w).writer();

f.writeln(myComplex);
f.close();
