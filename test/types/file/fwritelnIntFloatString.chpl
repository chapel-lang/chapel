var myInt = 123;
var myFloat = 99.9;
var myString = " ";

var f = open("_test_fwritelnIntFloatStringFile.txt", mode.w).writer();

f.writeln(myInt, myString, myFloat);
f.close();

