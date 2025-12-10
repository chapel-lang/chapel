use Helpers;

config param t = 1;
// only t==(1,5,9,) should suggest toByte
if t == 1 {
  testInit('a', c_char);
} else if t == 2 {
  testInitVal('a', c_char);
} else if t == 3 {
  testInit("hello", c_char);
} else if t == 4 {
  testInitVal("hello", c_char);
} else if t == 5 {
  testInit('a', int(8));
} else if t == 6 {
  testInitVal('a', int(8));
} else if t == 7 {
  testInit("hello", int(8));
} else if t == 8 {
  testInitVal("hello", int(8));
} else if t == 9 {
  testInit('a', uint(8));
} else if t == 10 {
  testInitVal('a', uint(8));
} else if t == 11 {
  testInit("hello", uint(8));
} else if t == 12 {
  testInitVal("hello", uint(8));
}

