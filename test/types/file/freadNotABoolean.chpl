var thisIsTrue: bool = true;

var f: file = new file(filename = "freadNotABoolean.txt", mode = "r");
f.open();

f.read(thisIsTrue);
writeln(thisIsTrue);

