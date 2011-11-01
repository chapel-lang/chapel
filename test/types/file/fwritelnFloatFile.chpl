var myFloat = 9.99;
var f = open("_test_fwritelnFloatFile.txt", mode.w).writer();

f.writeln(myFloat);
f.close();

