record R {
  var x: int;

  proc init() {
    writeln("In default initializer");
    this.x = 33;
  }

  proc init=(x: int) {
    writeln("In copy initializer");
    this.x = x;
  }
}

proc =(ref lhs: R, rhs: int) {
  lhs.x = rhs;
}

proc bar() {
  return (78, 45);
}

var myR: R;

(myR, _) = bar();

writeln(myR);
