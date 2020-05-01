record R {
  var field: int;
  proc init() {
    writeln("init()");
  }
  proc init(arg:int) {
    writeln("init(", arg, ")");
    this.field = arg;
  }
  proc init=(other: R) {
    writeln("init=(", other, ")");
    this.field = other.field;
  }
}
proc =(ref lhs: R, rhs: R) {
  writeln(lhs, " = ", rhs);
  lhs.field = rhs.field;
}

proc main() {
  var x:R;
  f();
  writeln(x);
  
  proc f() {
    x = new R(1);
  }
}

