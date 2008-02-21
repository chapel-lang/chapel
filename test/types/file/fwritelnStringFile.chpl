var myString = "You wear a nice shirt for a phone interview?";
var f: file = new file(filename = "_test_fwritelnStringFile.txt", mode = FileAccessMode.write);

f.open();
f.writeln(myString);
