var myInt = 123;
var myFloat = 99.9;
var myString = " ";

var f: file = file(filename = "_test_fwritelnIntFloatStringFile.txt", mode = "w");

f.open();
f.writeln(myInt, myString, myFloat);
