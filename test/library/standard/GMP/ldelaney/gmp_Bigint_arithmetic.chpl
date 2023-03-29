use BigInteger;

var a = new bigint(  0);
var b = new bigint(  2);
var c = new bigint(100);

add(a, a, b);                         // a =     2
add(a, a, 1000);                      // a =  1002
writeln(a);

sub(a, a, c);                         // a =   902
sub(a, a, 10);                        // a =   892
sub(a, 1000, a);                      // a =   108
writeln(a);

mul(a, a, b);                         // a =   216
mul(c, c, -2);                        // c =  -200
mul(a, a, 3);                         // a =   648
writeln(c);
writeln(a);

// a = a + (b * c)
addmul(a, b, c);                      // a =   248
addmul(a, b, 10);                     // a =   268
writeln(a);

// a = a - (b * c)
submul(a, b, c);                      // a =   668
submul(a, b, 4);                      // a =   660
writeln(a);

mul_2exp(a, c, 3);                    // a = -1600
writeln(a);

divQ2Exp(a, a, 3);                    //a =   -200
writeln(a);

neg(a, b);                            // a =    -2
writeln(a);

abs(a, a);                            // a =     2
writeln(a);
