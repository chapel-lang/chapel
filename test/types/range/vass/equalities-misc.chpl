// should be oblivious of idxType
var r1: range(int) = 0..1;
var r2: range(uint)= 0..1:uint;
writeln(r1==r2);
writeln(r1!=r2);

writeln( (0..(1:int)) == (0..(1:uint)) );

writeln( [0..(1:int)] == [0..(1:uint)] );
writeln( [0..(1:int)] != [0..(1:uint)] );
writeln();

// should discern the boundedness
const tests = (0..0, 0.., ..0, ..);
for param i1 in 0..3 do for param i2 in 0..3 {
  const cmp = (tests(i1) == tests(i2));
  writeln(cmp, if cmp == (i1 == i2) then " ok" else " error");
}
