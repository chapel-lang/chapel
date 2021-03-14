var x, y = 0;
proc A(i: int) ref {
  if i < 0 || i > 1 then
    halt("array access out of bounds");
  return if i == 0 then x else y;
}
A(0) = 1;
A(1) = 2;
writeln(A(0) + A(1));

