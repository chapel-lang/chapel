
record R {
  var A = foreach i in zip(1..4, foreach j in 1..4 do j) do i;
}

proc main() {
  var A = foreach i in zip(1..4, foreach j in 1..4 do j) do i;
  writeln(A);

  var r = new R();
  writeln(r);
}
