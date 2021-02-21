record R {
  var x: int;
}

// cross-type
proc _cast(type t: R, x: int) {
  return new R(x);
}

proc main() {
  var r = 1:R;
  writeln(r);
}
