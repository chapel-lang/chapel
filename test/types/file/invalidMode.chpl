use IO;

var myInt: int;
var f = open("anyFile.txt", ioMode.q).writer();

f.write(myInt);
