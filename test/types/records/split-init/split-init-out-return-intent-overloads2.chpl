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

proc b(out x: R)       ref { return glob; }
proc b(    x: R) const ref { return glob; }
proc test2() {
  writeln("test2");
  var x;
  b(x);
  writeln("x : ", x.type:string, " = ", x);
}
test2();
