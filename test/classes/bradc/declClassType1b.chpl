class C {
  var x = 10;
}

var globc: C = new C();

class D {
  var y = 20;
  var locc: C;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var d: D = new D();
  delete globc;
  globc = nil;
  d.doit;
  writeln(d);
  delete d;
}
