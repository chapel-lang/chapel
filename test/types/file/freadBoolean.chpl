use IO;

var thisIsTrue: bool = false;
var thisIsFalse: bool = true;

var f = open("freadBoolean.txt", iomode.r).reader();

f.read(thisIsTrue, thisIsFalse);
writeln(thisIsTrue);
writeln(thisIsFalse);
