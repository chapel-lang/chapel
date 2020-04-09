use IO;

var myInt = 123;
var f = open("_test_freadIntFailed.txt", iomode.cw).reader();

f.read(myInt);
