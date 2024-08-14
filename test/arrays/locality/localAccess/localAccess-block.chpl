use BlockDist;
config param POD = true;
config const read = true;

var Arr = createArray();

on Locales[numLocales-1] {
  if read then writeln(Arr.localAccess[0]); else Arr.localAccess[0] += 1;
}

proc createArray() {
  const D = {0..2} dmapped new blockDist({0..99});
  var A: [D] if POD then int else R;
  if POD then
    A = [1, 2, 3];
  else
    A = [new R(1), new R(2), new R(3)];
  return A;
}

record R {
  var x: int;

  operator +(ref lhs: R, rhs: int) {
    lhs.x += rhs;
    return lhs;
  }
}

