private use List;

config const testIters = 4;

class C {
  var x : int;
}

record R {
  var c : unmanaged C?;
  proc init() { }
  proc init(x:int) {
    c = new unmanaged C(x);
  }
  proc init=(other:R) {
    this.c = new unmanaged C(other.c!.x);
  }
  proc deinit() {
    if c != nil then delete c;
  }
}

//
// List currently uses a primitive move instead of an assignment internally,
// so this routine is unused.
//
proc =(ref LHS : R, rhs : R) {
  if LHS.c then delete LHS.c;
  LHS.c = new unmanaged C(rhs.c!.x);
}

proc main() {
  var L: list(R);

  for i in 1..testIters do L.append(new R(i));

  // A copy of R(4) is leaked.
  while !L.isEmpty() do L.pop(0);
}
