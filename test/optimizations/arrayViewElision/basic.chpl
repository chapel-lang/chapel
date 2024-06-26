var A: [1..10] int = 1;
var B: [1..10] int = 2;

proc test(dstRange, srcRange) {
  A[dstRange] = B[srcRange];
  writeln(A);
  A = 1; // reset
}

writeln("Set first two:");
test(1..2, 1..2);
test(1..2, 9..10);
test(1..2, ..2);
test(1..2, 9..);
test(..2, 1..2);
test(..2, 8..9);
test(..2, ..2);
test(..2, 9..);

writeln("Set last two:");
test(9..10, 1..2);
test(9..10, 9..10);
test(9..10, ..2);
test(9..10, 9..);
test(9.., 1..2);
test(9.., 9..10);
test(9.., ..2);
test(9.., 9..);

writeln("Set all:");
test(1..10, 1..10);
test(1..10, 1..);
test(1..10, ..);
test(1.., 1..10);
test(1.., 1..);
test(1.., ..);
test(.., 1..10);
test(.., 1..);
test(.., ..);
