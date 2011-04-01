proc main() {
  var b     : volatile bool;
  var b8    : volatile bool(8);
  var b16   : volatile bool(16);
  var b32   : volatile bool(32);
  var b64   : volatile bool(64);
  writeln("b = ", b);
  writeln("b8 = ", b8);
  writeln("b16 = ", b16);
  writeln("b32 = ", b32);
  writeln("b64 = ", b64);

  // int decls
  var i     : volatile int;
  var i8    : volatile int(8);
  var i16   : volatile int(16);
  var i32   : volatile int(32);
  var i64   : volatile int(64);
  writeln("i = ", i);
  writeln("i8 = ", i8);
  writeln("i16 = ", i16);
  writeln("i32 = ", i32);
  writeln("i64 = ", i64);

  // uint decls
  var ui    : volatile uint;
  var ui8   : volatile uint(8);
  var ui16  : volatile uint(16);
  var ui32  : volatile uint(32);
  var ui64  : volatile uint(64);
  writeln("ui = ", ui);
  writeln("ui8 = ", ui8);
  writeln("ui16 = ", ui16);
  writeln("ui32 = ", ui32);
  writeln("ui64 = ", ui64);

  // real decls
  var r     : volatile real;    // real32 and real
  var r32   : volatile real(32);
  var r64   : volatile real(64);
  writeln("r = ", r);
  writeln("r32 = ", r32);
  writeln("r64 = ", r64);

  // imag decls
  var img   : volatile imag;
  var img32 : volatile imag(32);
  var img64 : volatile imag(64);
  writeln("img = ", img);
  writeln("img32 = ", img);
  writeln("img64 = ", img);

  /*
  // complex decls  
  var cmplx : volatile complex;
  var cmplx64 : volatile complex(64);
  var cmplx128 : volatile complex(128);
  writeln("cmplx = ", cmplx);
  */
}

