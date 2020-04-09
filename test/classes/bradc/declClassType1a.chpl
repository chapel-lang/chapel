class C {
  var x = 10;
}

var globc: unmanaged C = new unmanaged C();

class D {
  var y = 20;
  var locc: unmanaged C?;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var d: unmanaged D = new unmanaged D();
  d.doit;
  writeln(d);
  delete d;
  delete globc;
}
