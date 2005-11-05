var myInt = 123;
var f: file = file(filename = "_test_freadIntFailed.txt", mode = "w");

f.open;
fread(f, myInt);
