// number of bytes to generate
config const n = 100_000;

proc test1() {
  var b = "hello ";
  b.appendCodepointValues(b"a"(0));
  b.appendCodepointValues(b"A"(0));
  b.appendCodepointValues(b"."(0));
  b.appendCodepointValues(b"9"(0));
  assert(b == "hello aA.9");
}
test1();

proc test2() {
  var b = "hello ";
  b.appendCodepointValues(b"a"(0), b"A"(0), b"."(0), b"9"(0));
  assert(b == "hello aA.9");
}
test2();

proc test3() {
  var str: string;
  for i in 0..<n {
    str.appendCodepointValues((i & 0xff): uint(8));
  }

  for (b,i) in zip(str.codepoints(), 0..) {
    assert(b == (i & 0xff): uint(8));
  }
}
test3();

proc test4() {
  var str: string;
  for i in 0..<n {
    str.appendCodepointValues((i & 0xff): uint(8), (i & 0xff): uint(8));
  }

  for (b,i) in zip(str.codepoints(), 0..) {
    const idx = i / 2;
    assert(b == (idx & 0xff): uint(8));
  }
}
test4();

proc test5() {
  // check handling of invalid unicode codepoints 
  var replacementChar: string;
  replacementChar.appendCodepointValues(0xFFFD);
  assert(replacementChar.codepoint(0) == 0xFFFD);
  var maximalChar: string;
  maximalChar.appendCodepointValues(0x10FFFF);
  assert(maximalChar.codepoint(0) == 0x10FFFF);
  var str: string;
  str.appendCodepointValues(0, 0x10FFFF, -1, 0x110000);
  assert(str.codepoint(0) == 0);
  assert(str.codepoint(1) == 0x10FFFF);
  assert(str.codepoint(2) == 0xFFFD); // replacement char
  assert(str.codepoint(3) == 0xFFFD); // replacement char
}
test5();

proc test6() {
  // check num codepoints calculation
  var str: string;
  assert(str.numCodepoints == 0);
  str.appendCodepointValues(0x00);
  assert(str.numCodepoints == 1);
  str.appendCodepointValues(0xFF);
  assert(str.numCodepoints == 2);
  str.appendCodepointValues(0x1234);
  assert(str.numCodepoints == 3);
  str.appendCodepointValues(0x10FFFF);
  assert(str.numCodepoints == 4);
}
test6();
