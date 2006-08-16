var myInt = 123;
var f: file = file(filename = "fwriteIntFailed.txt", mode = "r");

f.open;
var s: string = myInt;
fwrite(f, s);
