record R {
  var x:(int, int);
}


proc chooseRecord(ref A:R, ref B:R, chooseA:bool) ref
{
  if chooseA then return A;
  else return B;
}

var A:R;
var B:R;

A.x(1) = 1;
A.x(2) = 1;
B.x(1) = 2;
B.x(2) = 2;

chooseRecord(A, B, false).x(1) = 8;

chooseRecord(A, B, true).x(1) = 9;


writeln(A);
writeln(B);

