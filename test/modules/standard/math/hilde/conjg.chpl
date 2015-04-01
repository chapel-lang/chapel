// conjg.chpl
// 
// Test the complex conjugate function.
//

var co64a: complex(64) = 0.0:real(32) + 0.0i:imag(32);
var co64b: complex(64) = 0.0:real(32) - 0.0i:imag(32);
var co64c: complex(64) = -0.0:real(32) + 0.0i:imag(32);
var co64d: complex(64) = -0.0:real(32) - 0.0i:imag(32);
var co64e: complex(64) = 0.0:real(32) + 0.857219956i:imag(32);
var co64f: complex(64) = 0.0:real(32) - 0.171834252i:imag(32);
var co64g: complex(64) = -0.0:real(32) + 0.827383541i:imag(32);
var co64h: complex(64) = -0.0:real(32) - 0.649322591i:imag(32);
var co64i: complex(64) = 0.081886566:real(32) + 0.0i:imag(32);
var co64j: complex(64) = 0.978456473:real(32) - 0.0i:imag(32);
var co64k: complex(64) = -0.794269604:real(32) + 0.0i:imag(32);
var co64l: complex(64) = -0.299026801:real(32) - 0.0i:imag(32);
var co64m: complex(64) = 0.578021071:real(32) + 0.966449i:imag(32);
var co64n: complex(64) = 0.879427076:real(32) - 0.137026073i:imag(32);
var co64o: complex(64) = -0.836965158:real(32) + 0.398430675i:imag(32);
var co64p: complex(64) = -0.668876256:real(32) - 0.455488517i:imag(32);

var co128a: complex(128) = 0.0 + 0.0i;
var co128b: complex(128) = 0.0 - 0.0i;
var co128c: complex(128) = -0.0 + 0.0i;
var co128d: complex(128) = -0.0 - 0.0i;
var co128e: complex(128) = 0.0 + 0.376178713i;
var co128f: complex(128) = 0.0 - 0.185467349i;
var co128g: complex(128) = -0.0 + 0.712466175i;
var co128h: complex(128) = -0.0 - 0.062952648i;
var co128i: complex(128) = 0.9606238 + 0.0i;
var co128j: complex(128) = 0.557994836 - 0.0i;
var co128k: complex(128) = -0.704734138 + 0.0i;
var co128l: complex(128) = -0.761591082 - 0.0i;
var co128m: complex(128) = 0.052249788 + 0.831189191i;
var co128n: complex(128) = 0.59638208 - 0.048053546i;
var co128o: complex(128) = -0.821677148 + 0.050541908i;
var co128p: complex(128) = -0.3145087 - 0.267219244i;

writeln("co64a = ", co64a, " conjg(co64a) = ", conjg(co64a));
writeln("co64b = ", co64b, " conjg(co64b) = ", conjg(co64b));
writeln("co64c = ", co64c, " conjg(co64c) = ", conjg(co64c));
writeln("co64d = ", co64d, " conjg(co64d) = ", conjg(co64d));
writeln("co64e = ", co64e, " conjg(co64e) = ", conjg(co64e));
writeln("co64f = ", co64f, " conjg(co64f) = ", conjg(co64f));
writeln("co64g = ", co64g, " conjg(co64g) = ", conjg(co64g));
writeln("co64h = ", co64h, " conjg(co64h) = ", conjg(co64h));
writeln("co64i = ", co64i, " conjg(co64i) = ", conjg(co64i));
writeln("co64j = ", co64j, " conjg(co64j) = ", conjg(co64j));
writeln("co64k = ", co64k, " conjg(co64k) = ", conjg(co64k));
writeln("co64l = ", co64l, " conjg(co64l) = ", conjg(co64l));
writeln("co64m = ", co64m, " conjg(co64m) = ", conjg(co64m));
writeln("co64n = ", co64n, " conjg(co64n) = ", conjg(co64n));
writeln("co64o = ", co64o, " conjg(co64o) = ", conjg(co64o));
writeln("co64p = ", co64p, " conjg(co64p) = ", conjg(co64p));

writeln("co128a = ", co128a, " conjg(co128a) = ", conjg(co128a));
writeln("co128b = ", co128b, " conjg(co128b) = ", conjg(co128b));
writeln("co128c = ", co128c, " conjg(co128c) = ", conjg(co128c));
writeln("co128d = ", co128d, " conjg(co128d) = ", conjg(co128d));
writeln("co128e = ", co128e, " conjg(co128e) = ", conjg(co128e));
writeln("co128f = ", co128f, " conjg(co128f) = ", conjg(co128f));
writeln("co128g = ", co128g, " conjg(co128g) = ", conjg(co128g));
writeln("co128h = ", co128h, " conjg(co128h) = ", conjg(co128h));
writeln("co128i = ", co128i, " conjg(co128i) = ", conjg(co128i));
writeln("co128j = ", co128j, " conjg(co128j) = ", conjg(co128j));
writeln("co128k = ", co128k, " conjg(co128k) = ", conjg(co128k));
writeln("co128l = ", co128l, " conjg(co128l) = ", conjg(co128l));
writeln("co128m = ", co128m, " conjg(co128m) = ", conjg(co128m));
writeln("co128n = ", co128n, " conjg(co128n) = ", conjg(co128n));
writeln("co128o = ", co128o, " conjg(co128o) = ", conjg(co128o));
writeln("co128p = ", co128p, " conjg(co128p) = ", conjg(co128p));

