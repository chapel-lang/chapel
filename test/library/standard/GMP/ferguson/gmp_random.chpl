use GMP;

config const printrandom = false;

var r = new GMPRandom();
var b = new bigint();
var n = new bigint(10);
var x:uint = 0;

if printrandom then writeln(x);

x = r.urandomb_ui(10);

if printrandom then writeln(x);
if printrandom then writeln(b);

r.urandomb(b, 10);

if printrandom then writeln(b);

r.urandomm(b, n);

if printrandom then writeln(b);
