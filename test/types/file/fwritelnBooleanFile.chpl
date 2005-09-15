var myTrueBoolean: boolean = true;
var myFalseBoolean = false;
var aSpace = " ";

var f: file = file(filename = "_test_fwritelnBooleanFile.txt", mode = "w");

f.open;
fwriteln(f, myTrueBoolean, aSpace, myFalseBoolean);
