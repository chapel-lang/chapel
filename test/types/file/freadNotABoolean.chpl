var thisIsTrue: bool = true;

var f = open("freadNotABoolean.txt", "r").reader();

f.read(thisIsTrue);
writeln(thisIsTrue);

