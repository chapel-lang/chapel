config type dataType = string;
var x = "a1b2c3d4e5":dataType;

var y = "abcd abcd":dataType;

writeln(x.find("abcde":dataType, 0:byteIndex.. by 2));
writeln(x.find("12345":dataType, 0:byteIndex.. by 2));
writeln(x.find("12345":dataType, 1:byteIndex.. by 2));
writeln(y.find("dcba":dataType, 0:byteIndex.. by -1));
