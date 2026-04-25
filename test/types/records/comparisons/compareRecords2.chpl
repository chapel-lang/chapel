record R {
  var x: int;
  var y: int;
}

var myRs = [new R(x=42, y=42), new R(x=42, y=43), new R(x=43, y=42), new R(x=43, y=43)];

for i in 0..<myRs.size do
  for j in i..<myRs.size do
    testit(myRs(i), myRs(j));

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
