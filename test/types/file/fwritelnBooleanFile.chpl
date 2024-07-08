use IO;

var myTrueBoolean: bool = true;
var myFalseBoolean = false;
var aSpace = " ";

var f = open("_test_fwritelnBooleanFile.txt", ioMode.cw).writer(locking=false);

f.writeln(myTrueBoolean, aSpace, myFalseBoolean);

f.close();

