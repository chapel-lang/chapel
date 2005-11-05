var myInt: integer;
var f: file = file(filename = "anyFile.txt", mode = "q");

f.open;
fwrite(f, myInt);
