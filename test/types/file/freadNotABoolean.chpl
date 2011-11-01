var thisIsTrue: bool = true;

var f = open("freadNotABoolean.txt", mode.r).reader();

f.read(thisIsTrue);
writeln(thisIsTrue);

