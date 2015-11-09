record R {
  var n: int;
  proc R(n: int) {
    this.n = n;
    writeln("Construct R: ", this.n);
  }
  proc ~R() {
    writeln("Destroy R: ", this.n);
  }
}

proc chpl__autoCopy(arg: R) {
  writeln("init copy R: ", arg.n);
}

proc chpl__initCopy(arg: R) {
  writeln("init copy R: ", arg.n);
}

proc =(ref lhs:R, rhs:R) {
  writeln("= lhs: ", lhs.n, " rhs: ", rhs.n);
}


proc f() {
  var r1 = new R(1);
  var r2 = new R(2);
  r2 = r1; // here, r1 is an expiring value,
           // so the optimization should replace this with
           // destroy r2
           // move r2, r1
           // don't destroy r1
           // (destroy r2 which now contains 1)
}

f();
