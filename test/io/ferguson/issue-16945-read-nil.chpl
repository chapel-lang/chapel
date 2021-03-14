use IO;

class C {
  var x: int;
}

var f = openmem();

var x: owned C? = nil;
f.writer().write(x);

f.reader().read(x);
writeln(x);
