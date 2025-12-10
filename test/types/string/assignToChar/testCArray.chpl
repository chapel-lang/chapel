use Helpers;

config param t = 1;
// only t==(1,3,5,7,9,11,) should suggest toByte
if t == 1 {
  testCArray('a', c_char);
} else if t == 2 {
  testCArrayVal('a', c_char);
} else if t == 3 {
  testCArray("hello", c_char);
} else if t == 4 {
  testCArrayVal("hello", c_char);
} else if t == 5 {
  testCArray('a', int(8));
} else if t == 6 {
  testCArrayVal('a', int(8));
} else if t == 7 {
  testCArray("hello", int(8));
} else if t == 8 {
  testCArrayVal("hello", int(8));
} else if t == 9 {
  testCArray('a', uint(8));
} else if t == 10 {
  testCArrayVal('a', uint(8));
} else if t == 11 {
  testCArray("hello", uint(8));
} else if t == 12 {
  testCArrayVal("hello", uint(8));
}

