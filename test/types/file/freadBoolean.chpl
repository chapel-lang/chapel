var thisIsTrue: boolean = false;
var thisIsFalse: boolean = true;

var f: file = file(filename = "freadBoolean.txt", mode = "r");
f.open;

fread(f, thisIsTrue, thisIsFalse);
writeln(thisIsTrue);
writeln(thisIsFalse);
