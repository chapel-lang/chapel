var thisIsTrue: bool = true;

var f: file = file(filename = "freadNotABoolean.txt", mode = "r");
f.open;

fread(f, thisIsTrue);
writeln(thisIsTrue);

