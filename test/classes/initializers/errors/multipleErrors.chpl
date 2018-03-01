
proc printer(args...) {
  for a in args do writeln(a);
}

class C {
  var x : int;
  var y : real;

  proc init() {
    printer(x, y, this, this.foo());
    this.initDone();
  }
  
  proc foo() {
    return x*y;
  }
}
