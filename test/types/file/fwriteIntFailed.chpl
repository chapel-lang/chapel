use IO;

var myInt = 123;
var f = open("fwriteIntFailed.txt", iomode.r).writer();

f.write(myInt);
