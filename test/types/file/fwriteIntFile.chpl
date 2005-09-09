var myInt = 123;
var f: file = file(filename = "_test_fwriteIntFile.txt", mode = "w");

f.open;
fwrite(f, myInt);
