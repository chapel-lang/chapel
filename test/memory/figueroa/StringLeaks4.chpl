// In this test case, s2 should be freed after the outermost loop, but before
// the last writeln in foo.  At a minimum, valgrind should catch the case where
// s2 is freed within either loop.

use Memory;

config const n = 1;

proc foo () {
  var m1 = memoryUsed();
  var s1 = "Honey, I'm home!  Where's my dinner?",
      s2 = s1;
  for i in 1..2 do
    for j in 1..2 do
      if n > 0 then
        writeln(s2);
  var m2 = memoryUsed();
  writeln(s1);
  return m2:int - m1:int;
}

writeln("Amount of leaked memory after calling foo(): ", foo());
