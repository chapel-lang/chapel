use Helpers;

config param t = 1;
// only t==(1,5,9,) should suggest toByte
if t == 1 {
  testCall('a', c_char);
} else if t == 2 {
  testCallVal('a', c_char);
} else if t == 3 {
  testCall("hello", c_char);
} else if t == 4 {
  testCallVal("hello", c_char);
} else if t == 5 {
  testCall('a', int(8));
} else if t == 6 {
  testCallVal('a', int(8));
} else if t == 7 {
  testCall("hello", int(8));
} else if t == 8 {
  testCallVal("hello", int(8));
} else if t == 9 {
  testCall('a', uint(8));
} else if t == 10 {
  testCallVal('a', uint(8));
} else if t == 11 {
  testCall("hello", uint(8));
} else if t == 12 {
  testCallVal("hello", uint(8));
} else if t == 13 {
  testCallVal("", uint(8));
} else if t == 14 {
  testCallVal("", uint(8));
}

