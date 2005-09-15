var myString = "You wear a nice shirt for a phone interview?";
var f: file = file(filename = "_test_fwritelnStringFile.txt", mode = "w");

f.open;
fwriteln(f, myString);
