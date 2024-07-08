use IO;

var myInt = 123;
var f = open("fwriteIntFailed.txt", ioMode.r).writer(locking=false);

f.write(myInt);
