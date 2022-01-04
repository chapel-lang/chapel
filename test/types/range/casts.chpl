var r = 1..3;

testit(1..3);
testit(1..3 by 2);
testit(1..);
testit(..10);
testit(..);

proc testit(r) {
  writeln(r, " as: ");

  var ru = r: range(uint, boundedType=r.boundedType, stridable=r.stridable);
  writeln(ru.type: string, ": ", ru);

  var ri8 = r: range(int(8), boundedType=r.boundedType, stridable=r.stridable);
  writeln(ri8.type: string, ": ", ri8);

  writeln();
}
