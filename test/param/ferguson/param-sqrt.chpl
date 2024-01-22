proc checksqrt(param p) {
  param result = __primitive("sqrt", p);
  writef("sqrt(%.4n : %s) = %.20n : %s\n",
         p, p.type:string,
         result, result.type:string);
}

proc main() {
  writeln("real");
  param r32: real(32) = 2.25;
  checksqrt(r32);
  param r64: real(64) = 2.25;
  checksqrt(r64);

  writeln("imag");
  param m32: imag(32) = 2.25i;
  checksqrt(m32);
  param m64: imag(64) = 2.25i;
  checksqrt(m64);

  writeln("int");
  param i8: int(8) = 10;
  checksqrt(i8);
  param i16: int(16) = 10;
  checksqrt(i16);
  param i32: int(32) = 10;
  checksqrt(i32);
  param i64: int(64) = 10;
  checksqrt(i64);

  writeln("uint");
  param u8: uint(8) = 10;
  checksqrt(u8);
  param u16: uint(16) = 10;
  checksqrt(u16);
  param u32: uint(32) = 10;
  checksqrt(u32);
  param u64: uint(64) = 10;
  checksqrt(u64);
}
