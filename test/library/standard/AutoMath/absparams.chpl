param u8one:uint(8) = 1;         testabs(u8one);
param u16one:uint(16) = 1;       testabs(u16one);
param u32one:uint(32) = 1;       testabs(u32one);
param u64one:uint(64) = 1;       testabs(u64one);

param i8one:int(8) = 1;          testabs(i8one);
param i16one:int(16) = 1;        testabs(i16one);
param i32one:int(32) = 1;        testabs(i32one);
param i64one:int(64) = 1;        testabs(i64one);

param r32one:real(32) = 1.0;      testabs(r32one);
param r32negOne:real(32) = -1.0;  testabs(r32negOne);
param r64one:real(64) = 1.0;      testabs(r64one);
param r64negOne:real(64) = -1.0;  testabs(r64negOne);

param m32one:imag(32) = 1.0i;     testabs(m32one);
param m32negOne:imag(32) = -1.0i; testabs(m32negOne);
param m64one:imag(64) = 1.0i;     testabs(m64one);
param m64negOne:imag(64) = -1.0i; testabs(m64negOne);

proc testabs(param x) {
  param p = abs(x);
  assert(p == 1.0);
  // also check non-param version, just for good measure
  var v = x;
  assert(abs(v) == 1.0);
}
