record R {
  type t;

  proc init(type t) {
    this.t = t;
  }

  proc init() {
    this.t = int;
  }
}

var myR: R(int);
var myR2: R(int);
var myR3 = new R();

proc =(ref rhs: R(?t), lhs: R(t)) {
  rhs.t = lhs.t;
}

myR = myR2;

