use IO;

class C {
  var x: int;
}

var f = openMemFile();

var x = new owned C(1);
f.writer(locking=false).write("nil");

f.reader(locking=false).read(x);
writeln(x);
