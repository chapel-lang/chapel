record R {
  type t;

  proc init(type t) {
    this.t = t;
  }

  proc init() {
    this.t = int;
  }
}

var myR: R(int) = new R();
var myR2: R(int) = new R();
var myR3 = new R();

proc =(ref lhs: R(?t), rhs: R(t)) {
  lhs.t = rhs.t;
}

myR = myR2;

