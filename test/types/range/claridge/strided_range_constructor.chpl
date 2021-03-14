var R1 = 1..8 by 2;

var R2 = new range(int, BoundedRangeType.bounded, true, 1, 8, 2, 1, true);

writeln(R1);

writeln(R2);

writeln(R1 == R2);
