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

config const option = true;

proc main() {
  var x:R;
  f();
  x = new R(1);

  writeln(x);
 
  var y:R;
  {
    g();
    if option then
      y = new R(1);
    else
      y = new R(1);
  }
   
  proc f() {
    writeln(x);
  }
  
  proc g() {
    writeln(y);
  }

}

