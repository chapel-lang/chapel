record R {
  proc enterContext() ref {
    writeln("entering");
    return none;
  }

  proc exitContext(in err: owned Error?) {
    writeln("exiting");
  }
}

var r: R;

manage r {
  writeln("inside");
}
