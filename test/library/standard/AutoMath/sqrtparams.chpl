writeln("uint cases");
param u8one:uint(8) = 1;         testsqrt(u8one);
param u16one:uint(16) = 1;       testsqrt(u16one);
param u32one:uint(32) = 1;       testsqrt(u32one);
param u64one:uint(64) = 1;       testsqrt(u64one);
assert(sqrt(4:uint) == 2);

writeln("int cases");
param i8one:int(8) = 1;          testsqrt(i8one);
param i16one:int(16) = 1;        testsqrt(i16one);
param i32one:int(32) = 1;        testsqrt(i32one);
param i64one:int(64) = 1;        testsqrt(i64one);
assert(sqrt(4) == 2);

writeln("real cases");
param r32one:real(32) = 1.0;      testsqrt(r32one);
param r64one:real(64) = 1.0;      testsqrt(r64one);
assert(sqrt(4.0) == 2.0);
// note: sqrt( negative real ) gives an error
// but it can work if you cast to complex

writeln("imag cases");
param m32one:imag(32) = 2.0i;     testsqrt2(m32one);
param m32negOne:imag(32) = -2.0i; testsqrt2(m32negOne);
param m64one:imag(64) = 2.0i;     testsqrt2(m64one);
param m64negOne:imag(64) = -2.0i; testsqrt2(m64negOne);

writeln("complex cases");
param c64one:complex(64) = 1.0;       testsqrt(c64one);
param c64negOne:complex(64) = -2.0i;  testsqrt2(c64negOne);
param c128one:complex(128) = 2.0i;    testsqrt2(c128one);
param c128negOne:complex(128) = -1.0; testsqrtN1(c128negOne);
assert(sqrt(4.0:complex) == 2.0:complex);

proc testsqrt(param x) {
  param p = sqrt(x);
  compilerWarning("x = " + x:string + ":" + x.type:string + " sqrt = " +
      p:string + ":" + p.type:string);
  assert(p == 1.0);
  // also check non-param version, just for good measure
  var v = x;
  assert(sqrt(v) == 1.0);
}

proc testsqrtN1(param x) {
  param p = sqrt(x);
  compilerWarning("x = " + x:string + ":" + x.type:string + " sqrt = " +
      p:string + ":" + p.type:string);
  assert(p == 1.0i);
  // also check non-param version, just for good measure
  var v = x;
  assert(sqrt(v) == 1.0i);
}


proc testsqrt2(param x) {
  param p = sqrt(x);
  compilerWarning("x = " + x:string + ":" + x.type:string + " sqrt = " +
      p:string + ":" + p.type:string);
  assert(abs(p.re) == 1.0);
  assert(abs(p.im) == 1.0);
  // also check non-param version, just for good measure
  const v = x; // using lots of temporaries to avoid a compilation error
  const c = sqrt(v);
  const cIm = c.im;
  const cRe = c.re;
  assert(abs(cIm) == 1.0);
  assert(abs(cRe) == 1.0);
}
