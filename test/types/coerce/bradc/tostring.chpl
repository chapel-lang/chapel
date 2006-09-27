var btr: bool = true;
var bfl: bool = false;

var i08: int( 8) = 1;
var i16: int(16) = 2;
var i32: int(32) = 3;
var i64: int(64) = 4;
var idf: int     = 5;

var u08: uint( 8) = 6;
var u16: uint(16) = 7;
var u32: uint(32) = 8;
var u64: uint(64) = 9;
var udf: uint     = 10;

var f32: float(32) = 11.12:float(32);
var f64: float(64) = 13.14;
var fdf: float     = 15.16;

var c64: complex(64) = (17.18 + 19.20i):complex(64);
var c128: complex(128) = 21.22 + 23.24i;
var cdf: complex = 25.26 + 27.28i;

def dobool(x) {
  var s: string;
  s = x;
  writeln("s is: ", s);
  writeln();
}

def dotype(x: ?t) {
  var s: string;
  s = x;
  writeln("s is: ", s);
  var v = min(t);
  s = v;
  writeln("s is: ", s);
  v = max(t);
  s = v;
  writeln("s is: ", s);
  writeln();
}

dobool(btr);
dobool(bfl);

dotype(i08);
dotype(i16);
dotype(i32);
dotype(i64);
dotype(idf);

dotype(u08);
dotype(u16);
dotype(u32);
dotype(u64);
dotype(udf);

dotype(f32);
dotype(f64);
dotype(fdf);

/* no min/max on complex types
dotype(c64);
dotype(c128);
dotype(cdf);
*/
