use Helpers;

config param t = 1;
// only t==(1,5,9,) should suggest toByte
if t == 1 {
  testAssign('a', c_char);
} else if t == 2 {
  testAssignVal('a', c_char);
} else if t == 3 {
  testAssign("hello", c_char);
} else if t == 4 {
  testAssignVal("hello", c_char);
} else if t == 5 {
  testAssign('a', int(8));
} else if t == 6 {
  testAssignVal('a', int(8));
} else if t == 7 {
  testAssign("hello", int(8));
} else if t == 8 {
  testAssignVal("hello", int(8));
} else if t == 9 {
  testAssign('a', uint(8));
} else if t == 10 {
  testAssignVal('a', uint(8));
} else if t == 11 {
  testAssign("hello", uint(8));
} else if t == 12 {
  testAssignVal("hello", uint(8));
}

