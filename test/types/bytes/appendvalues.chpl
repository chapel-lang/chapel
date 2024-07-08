// number of bytes to generate
config const n = 100_000;

proc test1() {
  var b = b"hello ";
  b.appendByteValues(b"a"(0));
  b.appendByteValues(b"A"(0));
  b.appendByteValues(b"."(0));
  b.appendByteValues(b"9"(0));
  assert(b == b"hello aA.9");
}
test1();

proc test2() {
  var b = b"hello ";
  b.appendByteValues(b"a"(0), b"A"(0), b"."(0), b"9"(0));
  assert(b == b"hello aA.9");
}
test2();

proc test3() {
  var bts: bytes;
  for i in 0..<n {
    bts.appendByteValues((i & 0xff): uint(8));
  }

  for (b,i) in zip(bts, 0..) {
    assert(b == (i & 0xff): uint(8));
  }
}
test3();

proc test4() {
  var bts: bytes;
  for i in 0..<n {
    bts.appendByteValues((i & 0xff): uint(8), (i & 0xff): uint(8));
  }

  for (b,i) in zip(bts, 0..) {
    const idx = i / 2;
    assert(b == (idx & 0xff): uint(8));
  }
}
test4();
