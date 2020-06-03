use IO;

var myInt: int;
var f = open("anyFile.txt", iomode.q).writer();

f.write(myInt);
