use IO;

var myInt: int;
var f = open("freadNoInt.txt", iomode.r).reader();

f.read(myInt);
