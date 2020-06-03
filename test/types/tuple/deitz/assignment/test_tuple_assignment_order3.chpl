var A = [i in 1..4] i;

writeln(A);

(A(1..2), A(3..4)) = (A(3..4), A(1..2));

writeln(A);

proc test1() {
  var B = [i in 1..4] i;
  writeln(B);
  (B(1..2), B(3..4)) = (B(3..4), B(1..2));
  writeln(B);
}
test1();

proc test2() {
  var B = [i in 1..4] i;
  writeln(B);
  var (C, D) = (B(3..4), B(1..2));
  writeln(C);
  writeln(D);
}
test2();
