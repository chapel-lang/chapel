var thisIsTrue: bool = false;
var thisIsFalse: bool = true;

var f: file = new file(filename = "freadBoolean.txt", mode = FileAccessMode.read);
f.open();

f.read(thisIsTrue, thisIsFalse);
writeln(thisIsTrue);
writeln(thisIsFalse);
