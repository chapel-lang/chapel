
record R {
  var A = [i in zip(1..4, [j in 1..4] j)] i;
}

proc main() {
  var A = [i in zip(1..4, [j in 1..4] j)] i;
  writeln(A);

  var r = new R();
  writeln(r);

  var B = for i in zip(1..4, for j in 1..4 do j) do i;
  writeln(B);
}
