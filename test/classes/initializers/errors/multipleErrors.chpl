
proc printer(args...) {
  for a in args do writeln(a);
}

proc named(x) {
  writeln(x);
}

record C {
  var x : int;
  var y : real;

  proc init() {
    printer(x, y, this, this.foo());
    named(x=this);
    this.complete();
  }
  
  proc foo() {
    return x*y;
  }
}
