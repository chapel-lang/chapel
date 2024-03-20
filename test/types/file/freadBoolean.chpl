use IO;

var thisIsTrue: bool = false;
var thisIsFalse: bool = true;

var f = open("freadBoolean.txt", ioMode.r).reader(locking=false);

f.read(thisIsTrue, thisIsFalse);
writeln(thisIsTrue);
writeln(thisIsFalse);
