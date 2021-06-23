use BigInteger;

var a = new bigint(  0);
var b = new bigint(  2);
var c = new bigint(100);

a.add(a, b);                         // a =     2
a.add(a, 1000);                      // a =  1002
writeln(a);

a.sub(a, c);                         // a =   902
a.sub(a, 10);                        // a =   892
a.sub(1000, a);                      // a =   108
writeln(a);

a.mul(a, b);                         // a =   216
c.mul(c, -2);                        // c =  -200
a.mul(a, 3);                         // a =   648
writeln(c);
writeln(a);

// a = a + (b * c)
a.addmul(b, c);                      // a =   248
a.addmul(b, 10);                     // a =   268
writeln(a);

// a = a - (b * c)
a.submul(b, c);                      // a =   668
a.submul(b, 4);                      // a =   660
writeln(a);

a.mul_2exp(c, 3);                    // a = -1600
writeln(a);

a.divQ2Exp(a, 3);                    //a =   -200
writeln(a);

a.neg(b);                            // a =    -2
writeln(a);

a.abs(a);                            // a =     2
writeln(a);
