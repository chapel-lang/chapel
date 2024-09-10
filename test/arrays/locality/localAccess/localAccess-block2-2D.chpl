use BlockDist;
config param POD = true;
config const read = true;

var Arr = createArray();

on Locales[numLocales-1] {
  local { if read then writeln(Arr.localAccess[0,0]); else Arr.localAccess[0,0] += 1;  }
}

proc createArray() {
  const D = {0..99 by 50, 0..99 by 50} dmapped new blockDist({0..99, 0..99});
  var A: [D] if POD then int else R;
  if POD then
    A = reshape([1, 2, 3, 4], A.domain);
  else
    A = reshape([new R(1), new R(2), new R(3), new R(4)], A.domain);
  return A;
}

record R {
  var x: int;

  operator +(ref lhs: R, rhs: int) {
    lhs.x += rhs;
    return lhs;
  }
}

