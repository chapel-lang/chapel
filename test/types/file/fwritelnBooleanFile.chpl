var myTrueBoolean: bool = true;
var myFalseBoolean = false;
var aSpace = " ";

var f: file = file(filename = "_test_fwritelnBooleanFile.txt", mode = "w");

f.open();
f.writeln(myTrueBoolean, aSpace, myFalseBoolean);
