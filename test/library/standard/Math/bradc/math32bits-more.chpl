use Math;

var x = 0.333: real(32);

var o = exp2(x);
writeln("#bits in result = ", numBits(o.type));

var u = log2(x);
writeln("#bits in result = ", numBits(u.type));

var y = nearbyint(x);
writeln("#bits in result = ", numBits(y.type));

var aa = round(x);
writeln("#bits in result = ", numBits(aa.type));

var gg = tgamma(x);
writeln("#bits in result = ", numBits(gg.type));

var hh = trunc(x);
writeln("#bits in result = ", numBits(hh.type));
