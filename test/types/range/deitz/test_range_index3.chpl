proc testRangeIndexing(r1, r2) {
  write("(", r1, ")(", r2, ") = ");
  for i in r1(r2) do
    write(i, " ");
  writeln();
}

testRangeIndexing(1..10 by -3, 3..9);
testRangeIndexing(1..10 by -3, 3..8);
testRangeIndexing(1..10 by -3, 3..7);
testRangeIndexing(1..10 by -3, 3..6);

testRangeIndexing(1..265 by 131, 1..233:uint(8));

testRangeIndexing(1..11 by -3, 3..8 by -3);

testRangeIndexing(1..10 by 3, 1..9 by -3);
testRangeIndexing(1..16 by 3, 1..15 by -2);
