class C {
  var x = 10;
}

var globc: C = new unmanaged C();

class D {
  var y = 20;
  var locc: C;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var d: D = new unmanaged D();
  d.doit;
  writeln(d);
  delete d;
  delete globc;
}
