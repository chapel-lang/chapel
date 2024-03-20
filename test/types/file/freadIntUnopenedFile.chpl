use IO;

var myInt = 123;
var f : file;

var r = f.reader(locking=false);

r.read(myInt);
