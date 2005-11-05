var myInt: integer;
var f: file = file(filename = "freadNoInt.txt", mode = "r");

f.open;
fread(f, myInt);
