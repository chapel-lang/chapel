// deprecated in 1.31 by jade
use BigInteger;

const a = new bigint(5);
const b = new bigint(13);
const i: int = -17;
const u: uint = 17;
const numer = new bigint(20);
const denom = new bigint(10);
const denom_i: int = 10;
const mod = new bigint(2);
var res = new bigint();
var res2 = new bigint();
var res3 = new bigint();

writeln();
res.divexact(numer, denom);
writeln(numer, " / ", denom, " = ", res);
res.divexact(numer, denom_i);
writeln(numer, " / ", denom_i, " = ", res);

writeln();
res.powMod(b, a, mod);
writeln("(", b, " ^ ", a, ") % ", mod, " = ", res);
res.powMod(b, i, mod);
writeln("(", b, " ^ ", i, ") % ", mod, " = ", res);
res.powMod(b, u, mod);
writeln("(", b, " ^ ", u, ") % ", mod, " = ", res);

writeln();
res.pow(b, i);
writeln(b, " ^ ", i, " = ", res);
res.pow(b, u);
writeln(b, " ^ ", u, " = ", res);
res.pow(i, i);
writeln(i, " ^ ", i, " = ", res);
res.pow(u, u);
writeln(u, " ^ ", u, " = ", res);

writeln();
res.root(b, 3);
writeln("root(", b, ", 3) = ", res);

writeln();
res.rootrem(res2, b, 3);
writeln("rootrem(", b, ", 3) = ", res, ", ", res2);

writeln();
res.sqrt(b);
writeln("sqrt(", b, ") = ", res);

writeln();
res.sqrtrem(res2, b);
writeln("sqrtrem(", b, ") = ", res, ", ", res2);

writeln();
res.nextprime(b);
writeln("nextprime(", b, ") = ", res);

writeln();
res.gcd(a, b);
writeln("gcd(", a, ", ", b, ") = ", res);
res.gcd(a, i);
writeln("gcd(", a, ", ", i, ") = ", res);
res.gcd(a, u);
writeln("gcd(", a, ", ", u, ") = ", res);
res.gcd(a, b, res2, res3);
writeln("gcd(", a, ", ", b, ") = ", res, ", s=", res2, ", t=", res3);

writeln();
res.lcm(a, b);
writeln("lcm(", a, ", ", b, ") = ", res);
res.lcm(a, i);
writeln("lcm(", a, ", ", i, ") = ", res);
res.lcm(a, u);
writeln("lcm(", a, ", ", u, ") = ", res);

writeln();
res.invert(a, b);
writeln("invert(", a, ", ", b, ") = ", res);

writeln();
res.removeFactor(a, b);
writeln("removeFactor(", a, ", ", b, ") = ", res);

writeln();
res.fac(u);
writeln("fac(", u, ") = ", res);

writeln();
res.bin(a, u);
writeln("bin(", a, ", ", u, ") = ", res);
res.bin(u, u);
writeln("bin(", u, ", ", u, ") = ", res);

writeln();
res.fib(u);
writeln("fib(", u, ") = ", res);
res.fib2(res2, u);
writeln("fib2(", u, ") = ", res, ", ", res2);

writeln();
res.lucnum(u);
writeln("lucnum(", u, ") = ", res);
res.lucnum2(res2, u);
writeln("lucnum2(", u, ") = ", res , ", ", res2);

writeln();
res.add(a, b);
writeln(a, " + ", b, " = ", res);
res.add(a, i);
writeln(a, " + ", i, " = ", res);
res.add(a, u);
writeln(a, " + ", u, " = ", res);

writeln();
res.sub(a, b);
writeln(a, " - ", b, " = ", res);
res.sub(a, i);
writeln(a, " - ", i, " = ", res);
res.sub(a, u);
writeln(a, " - ", u, " = ", res);
res.sub(i, b);
writeln(i, " - ", b, " = ", res);
res.sub(u, b);
writeln(u, " - ", b, " = ", res);

writeln();
res.mul(a, b);
writeln(a, " * ", b, " = ", res);
res.mul(a, i);
writeln(a, " * ", i, " = ", res);
res.mul(a, u);
writeln(a, " * ", u, " = ", res);

writeln();
res = 0;
res.addmul(a, b);
writeln("addmul(", a, ", ", b, ") = ", res);
res = 0;
res.addmul(a, i);
writeln("addmul(", a, ", ", i, ") = ", res);
res = 0;
res.addmul(a, u);
writeln("addmul(", a, ", ", u, ") = ", res);

writeln();
res = 0;
res.submul(a, b);
writeln("submul(", a, ", ", b, ") = ", res);
res = 0;
res.submul(a, i);
writeln("submul(", a, ", ", i, ") = ", res);
res = 0;
res.submul(a, u);
writeln("submul(", a, ", ", u, ") = ", res);

writeln();
res.mul_2exp(a, u);
writeln("mul_2exp(", a, ", ", u, ") = ", res);

writeln();
res.neg(a);
writeln("neg(", a, ") = ", res);

writeln();
res.abs(a);
writeln("abs(", a, ") = ", res);

writeln();
res.divQ(a, b);
writeln("divQ(", a, ", ", b, ") = ", res);
res.divQ(a, u);
writeln("divQ(", a, ", ", u, ") = ", res);

writeln();
res.divR(a, b);
writeln("divR(", a, ", ", b, ") = ", res);
res.divR(a, u);
writeln("divR(", a, ", ", u, ") = ", res);

writeln();
res.divQR(res2, a, b);
writeln("divQR(", a, ", ", b, ") = ", res, ", ", res2);
res.divQR(res2, a, u);
writeln("divQR(", a, ", ", u, ") = ", res, ", ", res2);

writeln();
res.divQ2Exp(a, u);
writeln("divQ2Exp(", a, ", ", u, ") = ", res);
res.divR2Exp(a, u);
writeln("divR2Exp(", a, ", ", u, ") = ", res);

writeln();
res.mod(a, b);
writeln(a, " % ", b, " = ", res);
res.mod(a, u);
writeln(a, " % ", u, " = ", res);

writeln();
res.and(a, b);
writeln(a, " & ", b, " = ", res);

writeln();
res.ior(a, b);
writeln(a, " | ", b, " = ", res);

writeln();
res.xor(a, b);
writeln(a, " ^ ", b, " = ", res);

writeln();
res.com(a);
writeln("~", a, " = ", res);

