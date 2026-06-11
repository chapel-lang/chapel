proc move(in lhs: int, out rhs: int) {
  rhs = lhs;
}
var A, B: [1..10] int;
move(A, B);

