use IO;

var thisIsTrue: bool = true;

var f = open("freadNotABoolean.txt", iomode.r).reader();

f.read(thisIsTrue);
writeln(thisIsTrue);

