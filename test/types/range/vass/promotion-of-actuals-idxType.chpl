var r1: range(int) = 0..1;
var r2: range(uint) = 0:uint..1;
var r64: range(int(64)) = 0..1;

proc test1(i1:range(?IT,?,?), i2:range(IT,?,?)) {
  writeln((i1,i2));
}

proc test2(i:r64.type) {
  writeln(i);
}

test1(r64,r1);
test1(r64,r2);
test2(r1);
test2(r2);
