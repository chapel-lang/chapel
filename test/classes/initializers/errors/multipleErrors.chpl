
proc printer(args...) {
  for a in args do writeln(a);
}

record C {
  var x : int;
  var y : real;

  proc init() {
    printer(x, y, this, this.foo());
    this.complete();
  }
  
  proc foo() {
    return x*y;
  }
}
