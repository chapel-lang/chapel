use Sort;
use Map;

var m0 = new map(int, int);
var m1 = new map(int, int);
var m2 = new map(int, int);

m0[1] = 1;
m0[2] = 2;
m0[3] = 3;
m1 = m0;
m2[1] = -1;
m2[3] = -3;
m2[5] = -5;

m1 += m2; writeln(sorted(m1.toArray())); m1 = m0;
m1 |= m2; writeln(sorted(m1.toArray())); m1 = m0;
m1 &= m2; writeln(sorted(m1.toArray())); m1 = m0;
m1 -= m2; writeln(sorted(m1.toArray())); m1 = m0;
m1 ^= m2; writeln(sorted(m1.toArray())); m1 = m0;

