// deprecated by Vass in 1.32

writeln("/////////// ranges ///////////");

var r1 = 1..3;
var r2 = 4..8 by 2;
var r3 = 10..12;

writeln(r2 + 2);
r3 += 3; writeln(r3);
writeln(-2 + r2);
writeln(r3 - 4);
r3 -= 5; writeln(r3);

writeln(r1 * r2);
writeln(r1 ** 3);
writeln();

writeln("/////////// domain + and - ///////////");

var d1 = {1..3};  //wass needed?
var d2, d3: domain(string);
config var str = "xyz", str2 = "s2";

writeln(d2 + "hi1");
writeln("hi2" + d2);
d2 += str; writeln(d2);
d2 -= str; writeln(d2);
d2 += "hi4"; writeln(d2);
writeln(d2 + d3);
writeln(d2 - d3);
d3 += d2; writeln(d3);
d3 -= d2; writeln(d3);
d2 -= "hi4"; writeln(d2);
d2 = {"s1", str2};
writeln(d2 - str2);
writeln(d2 - {"s2"});
d2 -= str2; writeln(d2);
d2 -= "s1"; writeln(d2);
writeln();

writeln("/////////// domain | & ^ ///////////");

d2 = {"d2"};
writeln(d2 | d3);
d3 |= d2; writeln(d3);
writeln(d2 & d3);
d3 &= d2; writeln(d3);
writeln(d2 ^ d3);
d3 ^= d2; writeln(d3);
