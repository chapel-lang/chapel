use IO;

var myInt: int;
var f = open("freadNoInt.txt", ioMode.r).reader();

f.read(myInt);
