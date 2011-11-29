var thisIsTrue: bool = false;
var thisIsFalse: bool = true;

var f = open("freadBoolean.txt", mode.r).reader();

f.read(thisIsTrue, thisIsFalse);
writeln(thisIsTrue);
writeln(thisIsFalse);
