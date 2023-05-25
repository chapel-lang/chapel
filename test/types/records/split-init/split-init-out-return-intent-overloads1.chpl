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

proc a(out x: R) const ref { return glob; }
proc a(    x: R)       ref { return glob; }
proc test1() {
  writeln("test1");
  var x;
  a(x);
  writeln("x : ", x.type:string, " = ", x);
}
test1();
