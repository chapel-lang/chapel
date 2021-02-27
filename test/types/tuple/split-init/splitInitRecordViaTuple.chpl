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

operator :(from: int, type t:R) {
  var tmp: R = from;
  return tmp;
}

proc bar() {
  return (78, 45);
}

var myR: R;

(myR, _) = bar();

writeln(myR);
