
proc main() {
  var i = 4;
  var A = [i in 1..i] i;
  writeln(A);

  var B = for i in 1..i do [i in 1..i] i;
  writeln(B);

  var C = for i in 1..i do for i in 1..i do i;
  writeln(C);
}
