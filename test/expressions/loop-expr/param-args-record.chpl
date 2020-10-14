// Ensure that param formals retain their param-ness inside loop expressions.

class C {
  param val: int;
}

record R {
  forwarding var data; // or just `var data`

  proc init(param val: int) {
    this.data = forall j in 1..10 do new owned C(val);
  }
}

proc main() {
  param val = 8;
  var r = new R(val);
  writeln(r);
}
