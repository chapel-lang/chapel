use IO;

var myInt = 123;
var f = open("_test_freadIntFailed.txt", ioMode.cw).reader(locking=false);

f.read(myInt);
