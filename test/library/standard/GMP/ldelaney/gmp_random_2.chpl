use BigInteger;
use GMP;

config const printrandom = false;

var a  = new bigint( 10);
var b  = new bigint(128);

var r1 = new owned GMPRandom();
var r2 = new owned GMPRandom(true);
var r3 = new owned GMPRandom(a, 10, 10);
var r4 = new owned GMPRandom(8);

r1.seed(a);
r2.seed(10);

var x = r1.urandomb(32);

if printrandom then writeln(x);

x = r2.urandomm(1024);

if printrandom then writeln(x);

r3.urandomb(a, 32);

if printrandom then writeln(a);

r4.urandomm(a, b);

if printrandom then writeln(a);
