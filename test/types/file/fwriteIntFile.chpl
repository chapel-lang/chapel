use IO;

var myInt = 123;
var f = open("_test_fwriteIntFile.txt", ioMode.cw).writer(locking=false);

f.write(myInt);

f.close();
