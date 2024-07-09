record R {
  proc enterContext() {
    writeln("entering");
  }

  proc exitContext(in err: owned Error?) {
    writeln("exiting");
  }
}

var r: R;

manage r {
  writeln("inside");
}
