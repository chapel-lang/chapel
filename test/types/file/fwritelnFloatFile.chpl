var myFloat = 9.99;
var f: file = new file(filename = "_test_fwritelnFloatFile.txt", mode = FileAccessMode.write);

f.open();
f.writeln(myFloat);
