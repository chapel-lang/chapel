class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
}
operator R.=(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}


var glob: int;

proc c(out x: R,     y: R) const ref { return glob; }
proc c(    x: R, out y: R)       ref { return glob; }
proc test3() {
  writeln("test3");
  var x, y;
  c(x,y);
  writeln("x : ", x.type:string, " = ", x);
  writeln("y : ", y.type:string, " = ", y);
}
test3();
