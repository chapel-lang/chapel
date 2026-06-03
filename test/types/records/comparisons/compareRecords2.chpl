record R {
  var x: int;
  var y: int;
}

var myRs = [new R(x=42, y=42), new R(x=42, y=43), new R(x=43, y=42), new R(x=43, y=43)];

for r1 in myRs do
  for r2 in myRs do
    testit(r1, r2);

proc testit(r1, r2) {
  writeln(r1, " ?<===> ", r2);
  writeln(" == :", r1 == r2);
  writeln(" != :", r1 != r2);
  writeln(" < :", r1 < r2);
  writeln(" <= :", r1 <= r2);
  writeln(" > :", r1 > r2);
  writeln(" >= :", r1 >= r2);
  writeln();
}
