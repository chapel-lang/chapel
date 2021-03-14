record R {
  var x:(int, int);
}


proc chooseRecord(ref A:R, ref B:R, chooseA:bool) ref
{
   if chooseA then return A;
   else return B;
}

proc updateAndPrint(ref A:R, ref B:R, C:R, D:R)
{
  A.x(0) = 8;
  B.x(0) = 9;
  writeln(A);
  writeln(B);
  writeln(C);
  writeln(D);
}

var A:R;
var B:R;

A.x(0) = 1;
A.x(1) = 1;
B.x(0) = 2;
B.x(1) = 2;

updateAndPrint(A, B, chooseRecord(A, B, false), chooseRecord(A, B, true));

// Note -- this test prints out
// (x = (8, 1))
// (x = (9, 2))
// (x = (2, 2))
// (x = (1, 1))
//
// At present, a ref function used in a value context runs the
// "value version" of that function, which has setter=false and
// does not return a ref.
//
// Alternatively, it could return a ref if setter is not used.
// It probably should not return a ref if setter is used, because
// we want to support a pattern like this:
//
// sparseStructure.this(i) ref {
//   if hasIndex(i) then return data(getIndex(i));
//   else if setter then {
//      addIndexToData(i);
//      return data(getIndex(i));
//   } else return 0;
// }
//
// Note in that pattern that it wouldn't make any sense
// to return 0 if setter was not queried, since you can't
// return an lvalue to 0.

