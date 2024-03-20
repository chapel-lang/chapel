proc checkabs(param p) {
  param result = __primitive("abs", p);
  writef("abs(%.4n : %s) = %.20n : %s\n",
         p, p.type:string,
         result, result.type:string);
}

proc main() {
  writeln("real");
  param r32: real(32) = -2.25;
  checkabs(r32);
  checkabs(-r32);
  param r64: real(64) = -2.25;
  checkabs(r64);
  checkabs(-r64);

  writeln("imag");
  param m32: imag(32) = -2.25i;
  checkabs(m32);
  checkabs(-m32);
  param m64: imag(64) = -2.25i;
  checkabs(m64);
  checkabs(-m64);
  
  writeln("complex");
  param c64: complex(64) = 3.0 - 4.0i;
  checkabs(c64);
  param c128: complex(128) = 3.0 - 4.0i;
  checkabs(c128);

  writeln("int");
  param i8: int(8) = -10;
  checkabs(i8);
  checkabs(-i8);
  param i16: int(16) = -10;
  checkabs(i16);
  checkabs(-i16);
  param i32: int(32) = -10;
  checkabs(i32);
  checkabs(-i32);
  param i64: int(64) = -10;
  checkabs(i64);
  checkabs(-i64);

  writeln("uint");
  param u8: uint(8) = 10;
  checkabs(u8);
  param u16: uint(16) = 10;
  checkabs(u16);
  param u32: uint(32) = 10;
  checkabs(u32);
  param u64: uint(64) = 10;
  checkabs(u64);
}
