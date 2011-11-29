proc testRangeIndexing(r1, r2) {
  write("(", r1, ")(", r2, ") = ");
  for i in r1(r2) do
    write(i, " ");
  writeln();
}

testRangeIndexing(1..10 by 3, 5..8);
testRangeIndexing(1..10 by 3, 4..7);
testRangeIndexing(1..10 by 3, 6..20);
testRangeIndexing(1..10 by 3, 1..0);
testRangeIndexing(1..10 by 3, 2..20 by 2);
testRangeIndexing(1..10 by 3, 2..11 by 3);
testRangeIndexing(1..10 by 3, 1.. by 2);
testRangeIndexing(1.., ..10);
