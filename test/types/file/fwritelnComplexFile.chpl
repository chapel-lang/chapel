var myComplex: complex = 1.2 + 2.3i;

var f: file = new file(filename = "_test_fwritelnComplexFile.txt", mode = "w");

f.open();
f.writeln(myComplex);
