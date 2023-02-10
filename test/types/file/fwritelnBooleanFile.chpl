use IO;

var myTrueBoolean: bool = true;
var myFalseBoolean = false;
var aSpace = " ";

var f = open("_test_fwritelnBooleanFile.txt", ioMode.cw).writer();

f.writeln(myTrueBoolean, aSpace, myFalseBoolean);

f.close();

