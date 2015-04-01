// abs.chpl
// 
// Test the abs function.
//  (Does not test the param version.)
//

var ui8a: uint(8) = 0;
var ui8b: uint(8) = 32;
var ui16a: uint(16) = 0;
var ui16b: uint(16) = 42441;
var ui32a: uint(32) = 0;
var ui32b: uint(32) = 3997024116;
var ui64a: uint(64) = 0;
var ui64b: uint(64) = 17914497524592412092;

var i8a: int(8) = -44;
var i8b: int(8) = 0;
var i8c: int(8) = 5;
var i16a: int(16) = -3322;
var i16b: int(16) = 0;
var i16c: int(16) = 5120;
var i32a: int(32) = -472289673;
var i32b: int(32) = 0;
var i32c: int(32) = 666765169;
var i64a: int(64) = -1996069246905712482;
var i64b: int(64) = 0;
var i64c: int(64) = 2625360699462582167;

var re32a: real(32) = -0.653338667:real(32);
var re32b: real(32) = -0.0:real(32);
var re32c: real(32) = 0.0:real(32);
var re32d: real(32) = 0.814420537:real(32);

var re64a: real(64) = -0.988837303;
var re64b: real(64) = -0.0;
var re64c: real(64) = 0.0;
var re64d: real(64) = 0.153876293;

var im32a: imag(32) = -0.794258792i:imag(32);
var im32b: imag(32) = -0.0i:imag(32);
var im32c: imag(32) = 0.0i:imag(32);
var im32d: imag(32) = 0.749659374i:imag(32);

var im64a: imag(64) = -0.172455709i;
var im64b: imag(64) = -0.0i;
var im64c: imag(64) = 0.0i;
var im64d: imag(64) = 0.770481847i;

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

writeln("ui8a = ", ui8a, " abs(ui8a) = ", abs(ui8a));
writeln("ui8b = ", ui8b, " abs(ui8b) = ", abs(ui8b));
writeln("ui16a = ", ui16a, " abs(ui16a) = ", abs(ui16a));
writeln("ui16b = ", ui16b, " abs(ui16b) = ", abs(ui16b));
writeln("ui32a = ", ui32a, " abs(ui32a) = ", abs(ui32a));
writeln("ui32b = ", ui32b, " abs(ui32b) = ", abs(ui32b));
writeln("ui64a = ", ui64a, " abs(ui64a) = ", abs(ui64a));
writeln("ui64b = ", ui64b, " abs(ui64b) = ", abs(ui64b));

writeln("i8a = ", i8a, " abs(i8a) = ", abs(i8a));
writeln("i8b = ", i8b, " abs(i8b) = ", abs(i8b));
writeln("i8c = ", i8c, " abs(i8c) = ", abs(i8c));
writeln("i16a = ", i16a, " abs(i16a) = ", abs(i16a));
writeln("i16b = ", i16b, " abs(i16b) = ", abs(i16b));
writeln("i16c = ", i16c, " abs(i16c) = ", abs(i16c));
writeln("i32a = ", i32a, " abs(i32a) = ", abs(i32a));
writeln("i32b = ", i32b, " abs(i32b) = ", abs(i32b));
writeln("i32c = ", i32c, " abs(i32c) = ", abs(i32c));
writeln("i64a = ", i64a, " abs(i64a) = ", abs(i64a));
writeln("i64b = ", i64b, " abs(i64b) = ", abs(i64b));
writeln("i64c = ", i64c, " abs(i64c) = ", abs(i64c));

writeln("re32a = ", re32a, " abs(re32a) = ", abs(re32a));
writeln("re32b = ", re32b, " abs(re32b) = ", abs(re32b));
writeln("re32c = ", re32c, " abs(re32c) = ", abs(re32c));
writeln("re32d = ", re32d, " abs(re32d) = ", abs(re32d));

writeln("re64a = ", re64a, " abs(re64a) = ", abs(re64a));
writeln("re64b = ", re64b, " abs(re64b) = ", abs(re64b));
writeln("re64c = ", re64c, " abs(re64c) = ", abs(re64c));
writeln("re64d = ", re64d, " abs(re64d) = ", abs(re64d));

writeln("im32a = ", im32a, " abs(im32a) = ", abs(im32a));
writeln("im32b = ", im32b, " abs(im32b) = ", abs(im32b));
writeln("im32c = ", im32c, " abs(im32c) = ", abs(im32c));
writeln("im32d = ", im32d, " abs(im32d) = ", abs(im32d));

writeln("im64a = ", im64a, " abs(im64a) = ", abs(im64a));
writeln("im64b = ", im64b, " abs(im64b) = ", abs(im64b));
writeln("im64c = ", im64c, " abs(im64c) = ", abs(im64c));
writeln("im64d = ", im64d, " abs(im64d) = ", abs(im64d));

writeln("co64a = ", co64a, " abs(co64a) = ", abs(co64a));
writeln("co64b = ", co64b, " abs(co64b) = ", abs(co64b));
writeln("co64c = ", co64c, " abs(co64c) = ", abs(co64c));
writeln("co64d = ", co64d, " abs(co64d) = ", abs(co64d));
writeln("co64e = ", co64e, " abs(co64e) = ", abs(co64e));
writeln("co64f = ", co64f, " abs(co64f) = ", abs(co64f));
writeln("co64g = ", co64g, " abs(co64g) = ", abs(co64g));
writeln("co64h = ", co64h, " abs(co64h) = ", abs(co64h));
writeln("co64i = ", co64i, " abs(co64i) = ", abs(co64i));
writeln("co64j = ", co64j, " abs(co64j) = ", abs(co64j));
writeln("co64k = ", co64k, " abs(co64k) = ", abs(co64k));
writeln("co64l = ", co64l, " abs(co64l) = ", abs(co64l));
writeln("co64m = ", co64m, " abs(co64m) = ", abs(co64m));
writeln("co64n = ", co64n, " abs(co64n) = ", abs(co64n));
writeln("co64o = ", co64o, " abs(co64o) = ", abs(co64o));
writeln("co64p = ", co64p, " abs(co64p) = ", abs(co64p));

writeln("co128a = ", co128a, " abs(co128a) = ", abs(co128a));
writeln("co128b = ", co128b, " abs(co128b) = ", abs(co128b));
writeln("co128c = ", co128c, " abs(co128c) = ", abs(co128c));
writeln("co128d = ", co128d, " abs(co128d) = ", abs(co128d));
writeln("co128e = ", co128e, " abs(co128e) = ", abs(co128e));
writeln("co128f = ", co128f, " abs(co128f) = ", abs(co128f));
writeln("co128g = ", co128g, " abs(co128g) = ", abs(co128g));
writeln("co128h = ", co128h, " abs(co128h) = ", abs(co128h));
writeln("co128i = ", co128i, " abs(co128i) = ", abs(co128i));
writeln("co128j = ", co128j, " abs(co128j) = ", abs(co128j));
writeln("co128k = ", co128k, " abs(co128k) = ", abs(co128k));
writeln("co128l = ", co128l, " abs(co128l) = ", abs(co128l));
writeln("co128m = ", co128m, " abs(co128m) = ", abs(co128m));
writeln("co128n = ", co128n, " abs(co128n) = ", abs(co128n));
writeln("co128o = ", co128o, " abs(co128o) = ", abs(co128o));
writeln("co128p = ", co128p, " abs(co128p) = ", abs(co128p));

