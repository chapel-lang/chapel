var myFloat: float;
var f: file = file(filename = "freadNoFloat.txt", mode = "r");

f.open;
fread(f, myFloat);
