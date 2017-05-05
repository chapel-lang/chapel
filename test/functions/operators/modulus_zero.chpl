/* Tests for modulus div-by-zero error */

config param overload = 1;

var int1 = 1: int,
    int0 = 0: int,
    uint1 = 1: uint,
    uint0 = 0: uint,
    real1 = 1: real,
    real0 = 0: real;

param paramint1 = 1: int,
      paramint0 = 0: int,
      paramuint1 = 1: uint,
      paramuint0 = 0: uint;

record R {
  var x = 1;
}
/* Define this so that %= is automatically defined */
proc %(lhs: R, rhs: R) return new R(lhs.x % rhs.x);

var rec1 = new R(1),
    rec0 = new R(0);


select overload {
  when 1 { //proc %(a: int(?w), b: int(w))
    var c = int1 % int0;
  }
  when 2 { //proc %(a: uint(?w), b: uint(w))
    var c = uint1 % uint0;
  }
  when 3 { //proc %(param a: int(?w), param b: int(w)) param
    var c = paramint1 % paramint0;
  }
  when 4 { //proc %(param a: uint(?w), param b: uint(w)) param
    var c = paramuint1 % paramuint0;
  }
  when 5 { //proc %=(ref lhs:int(?w), rhs:int(w))
    int1 %= int0;
  }
  when 6 { //proc %=(ref lhs:uint(?w), rhs:uint(w))
    uint1 %= uint0;
  }
  when 7 { //proc %=(ref lhs, rhs)
    rec1 %= rec0;
  }
  when 8 { //proc %(a: uint(64), param b: uint(64))
    var c = uint1 % paramuint0;
  }
  when 9 { //proc %(param a: uint(64), b: uint(64))
    var c = paramuint1 % uint0;
  }
  otherwise {
    compilerError('Unsupported overload value');
  }
}
