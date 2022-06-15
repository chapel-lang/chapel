
use IO;

record A {
  var x : int;

  proc readWriteThis(f) {
    if f.writing then f.write(x);
    else f.read(x);
  }
}

record B {
  var y : real;
}

proc B.readWriteThis(f) {
  if f.writing then f.write(y);
  else f.read(y);
}

proc main() {
  var f = openmem();
  {
    var w = f.writer();
    var a = new A(42);
    var b = new B(100);
    w.writeln(a);
    w.writeln(b);
  }
  {
    var r = f.reader();
    var contents : string;
    r.readstring(contents);
    writeln("===== Wrote: =====");
    writeln(contents);
    writeln("==================");
  }
  {
    var r = f.reader();
    var a : A;
    var b : B;
    r.readln(a);
    r.readln(b);
    writeln("===== Got: =====");
    writeln(a);
    writeln(b);
    writeln("================");
  }
}
