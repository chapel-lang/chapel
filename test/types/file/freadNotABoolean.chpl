use IO;

var thisIsTrue: bool = true;

var f = open("freadNotABoolean.txt", ioMode.r).reader(locking=false);

f.read(thisIsTrue);
writeln(thisIsTrue);

