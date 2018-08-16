class C {
  var x : int;
}

record R {
  var c : unmanaged C;
  proc init() { }
  proc init(x:int) {
    c = new unmanaged C(x);
  }
  proc init(other:R) {
    this.c = new unmanaged C(other.c.x);
  }
  proc deinit() {
    if c != nil then delete c;
  }
}

proc =(ref LHS : R, rhs : R) {
  if LHS.c then delete LHS.c;
  LHS.c = new unmanaged C(rhs.c.x);
}

proc main() {
  var A = for i in 1..4 do new R(i);

  // A copy of R(4) is leaked
  while A.size > 0 do A.pop_front();
}
