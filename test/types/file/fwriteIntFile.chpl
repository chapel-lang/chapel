var myInt = 123;
var f: file = new file(filename = "_test_fwriteIntFile.txt", mode = FileAccessMode.write);

f.open();
f.write(myInt);
