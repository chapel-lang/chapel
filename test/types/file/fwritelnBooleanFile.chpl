var myTrueBoolean: bool = true;
var myFalseBoolean = false;
var aSpace = " ";

var f: file = new file(filename = "_test_fwritelnBooleanFile.txt", mode = FileAccessMode.write);

f.open();
f.writeln(myTrueBoolean, aSpace, myFalseBoolean);
