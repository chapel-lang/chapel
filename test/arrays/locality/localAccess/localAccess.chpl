config param POD = true;

config const read = true;

var Arr = createArray();

on Locales[numLocales-1] {
  if read then writeln(Arr.localAccess[0]); else Arr.localAccess[0] += 1;
}

proc createArray() {
  if POD then
    return [1, 2, 3];
  else
    return [new R(1), new R(2), new R(3)];
}

record R {
  var x: int;

  operator +(ref lhs: R, rhs: int) {
    lhs.x += rhs;
    return lhs;
  }
}

