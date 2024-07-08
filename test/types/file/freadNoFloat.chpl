use IO;

var myFloat: real;
var f = open("freadNoFloat.txt", ioMode.r).reader(locking=false);

f.read(myFloat);

