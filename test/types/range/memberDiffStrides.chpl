var r1 = 1..10;
var r2 = 1..10 by  1;
var r3 = 1..10 by -1;
var r4 = 1..10 by  2;
var r5 = 1..10 by -2;

var s1 = 3..6;
var s2 = 3..6 by  1;
var s3 = 3..6 by -1;
var s4 = 3..6 by  2;
var s5 = 3..6 by -2;

var t1 = 4..6;
var t2 = 4..6 by  1;
var t3 = 4..6 by -1;
var t4 = 4..6 by  2;
var t5 = 4..6 by -2;

proc test(x, y) {
  writeln(x, ".contains(", y, ") = ", x.contains(y));
  writeln(y, ".contains(", x, ") = ", y.contains(x));
}

test(r1, r1);
test(r1, r2);
test(r1, r3);
test(r1, r4);
test(r1, r5);
test(r1, s1);
test(r1, s2);
test(r1, s3);
test(r1, s4);
test(r1, s5);
test(r1, t1);
test(r1, t2);
test(r1, t3);
test(r1, t4);
test(r1, t5);

test(r2, r2);
test(r2, r3);
test(r2, r4);
test(r2, r5);
test(r2, s1);
test(r2, s2);
test(r2, s3);
test(r2, s4);
test(r2, s5);
test(r2, t1);
test(r2, t2);
test(r2, t3);
test(r2, t4);
test(r2, t5);

test(r3, r3);
test(r3, r4);
test(r3, r5);
test(r3, s1);
test(r3, s2);
test(r3, s3);
test(r3, s4);
test(r3, s5);
test(r3, t1);
test(r3, t2);
test(r3, t3);
test(r3, t4);
test(r3, t5);

test(r4, r4);
test(r4, r5);
test(r4, s1);
test(r4, s2);
test(r4, s3);
test(r4, s4);
test(r4, s5);
test(r4, t1);
test(r4, t2);
test(r4, t3);
test(r4, t4);
test(r4, t5);

test(r5, r5);
test(r5, s1);
test(r5, s2);
test(r5, s3);
test(r5, s4);
test(r5, s5);
test(r5, t1);
test(r5, t2);
test(r5, t3);
test(r5, t4);
test(r5, t5);

test(s1, s1);
test(s1, s2);
test(s1, s3);
test(s1, s4);
test(s1, s5);
test(s1, t1);
test(s1, t2);
test(s1, t3);
test(s1, t4);
test(s1, t5);

test(s2, s2);
test(s2, s3);
test(s2, s4);
test(s2, s5);
test(s2, t1);
test(s2, t2);
test(s2, t3);
test(s2, t4);
test(s2, t5);

test(s3, s3);
test(s3, s4);
test(s3, s5);
test(s3, t1);
test(s3, t2);
test(s3, t3);
test(s3, t4);
test(s3, t5);

test(s4, s4);
test(s4, s5);
test(s4, t1);
test(s4, t2);
test(s4, t3);
test(s4, t4);
test(s4, t5);

test(s5, s5);
test(s5, t1);
test(s5, t2);
test(s5, t3);
test(s5, t4);
test(s5, t5);

test(t1, t1);
test(t1, t2);
test(t1, t3);
test(t1, t4);
test(t1, t5);

test(t2, t2);
test(t2, t3);
test(t2, t4);
test(t2, t5);

test(t3, t3);
test(t3, t4);
test(t3, t5);

test(t4, t4);
test(t4, t5);

test(t5, t5);

