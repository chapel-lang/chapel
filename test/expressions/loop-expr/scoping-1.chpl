
proc main() {
  var i = 4;
  var A = [i in 1..i] i;
  writeln(A);

  var B = for i in 1..i do [i in 1..i] i;
  writeln(B);

  var C = for i in 1..i do for i in 1..i do i;
  writeln(C);

  var D = foreach i in 1..i do [i in 1..i] i;
  writeln(D);

  var E = foreach i in 1..i do for i in 1..i do i;
  writeln(E);

  var F = foreach i in 1..i do foreach i in 1..i do i;
  writeln(F);
}
