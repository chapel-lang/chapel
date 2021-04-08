record R {
  var x: int;
}

// cross-type
operator :(x: int, type t: R) {
  return new R(x);
}

proc main() {
  var r = 1:R;
  writeln(r);
}
