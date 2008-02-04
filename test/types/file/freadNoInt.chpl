var myInt: int;
var f: file = new file(filename = "freadNoInt.txt", mode = "r");

f.open();
f.read(myInt);
