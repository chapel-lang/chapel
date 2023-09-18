class C {
  var x = 10;
}

var globc: borrowed C? = nil;

class D {
  var y = 20;
  var locc: borrowed C?;

  proc doit {
    locc = globc;
  }
}

proc main() {
  var dOwn = new owned D();
  var d: borrowed D = dOwn.borrow();

  d.doit;
  writeln(d);
}
