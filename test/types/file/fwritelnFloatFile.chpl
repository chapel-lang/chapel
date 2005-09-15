var myFloat = 9.99;
var f: file = file(filename = "_test_fwritelnFloatFile.txt", mode = "w");

f.open;
fwriteln(f, myFloat);
