
record R {
  var x : int;
}

class C {
  param t;
  var v = new R();

  proc init() {
    t = new R();
    init this;
    v = new R();
  }
}

proc main() {
  var c = new owned C();
}
