use IO;

class C {
  var x: int;
}

var f = openMemFile();

var x: owned C? = nil;
f.writer(locking=false).write(x);

f.reader(locking=false).read(x);
writeln(x);
